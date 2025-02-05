#include "geometryengine.h"

#include <filesystem>
#include <iostream>

#include <QVector2D>
#include <QVector3D>

GeometryEngine::GeometryEngine()
{
    initializeOpenGLFunctions();
}

void GeometryEngine::setDirectory(std::filesystem::path& path) {
    directory_ = path;

    initModelGeometry();
}

GeometryEngine::~GeometryEngine()
{
    for(Mesh* mesh : mesh_) {
        for(QOpenGLTexture* texture : mesh->textures) {
            delete texture;
        }
        delete mesh;
    }
}

void GeometryEngine::initModelGeometry() {

    bool found = false;
    std::filesystem::path m_path;
    for(const auto& entry : std::filesystem::directory_iterator(directory_)) {
        // std::cout << entry.path() << " " << entry.path().extension() << std::endl;
        if(entry.path().extension() == ".dae") {
            m_path = entry.path();
            found = true;
        }
    }

    if(!found) return;

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(m_path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    // aiProcess_OptimizeMeshes

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "GeometryEngine::initModelGeometry Unable to load Assimp model: " << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);
}

void GeometryEngine::processNode(aiNode* node, const aiScene* scene) {
    std::cout << "GeometryEngine::processNode found " << node->mNumMeshes << " meshes." << std::endl;
    std::cout << "GeometryEngine::processNode found " << node->mNumChildren << " children." << std::endl;

    // Process each mesh
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        // if(i == 16){
        aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(ai_mesh, scene);
        // }
    }

    // Process children recursively
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void GeometryEngine::processMesh(aiMesh* ai_mesh, const aiScene* scene) {
    // Store aiMesh elements in custom object
    Mesh* mesh = new Mesh();

    // Process mesh vertices
    std::cout << "GeometryEngine::processNode found " << ai_mesh->mNumVertices << " vertices in current mesh." << std::endl;
    // std::cout << "Positions: " << std::endl;

    // Skip meshes that are under the surface
    if(ai_mesh->mNumVertices > 0) {
        if(ai_mesh->mVertices[0].y < -8.0f) {
            return;
        }
    }

    for(unsigned int j = 0; j < ai_mesh->mNumVertices; j++) {
        // Vertex coordinates
        float x = ai_mesh->mVertices[j].x / 256;
        float y = ai_mesh->mVertices[j].y / 256;
        float z = ai_mesh->mVertices[j].z / 256;
        QVector3D position(x, y, z);

        // std::cout << position.x() << " " << position.y() << " " << position.z() << std::endl;

        // Texture coordinates
        QVector2D tex_coords(0.0f, 0.0f);
        if(ai_mesh->mTextureCoords[0]) {
            tex_coords = {
                ai_mesh->mTextureCoords[0][j].x,
                ai_mesh->mTextureCoords[0][j].y
            };
        }

        VertexData v_data;
        v_data.position = position;
        v_data.tex_coords = tex_coords;
        mesh->vertices.push_back(v_data);
    }

    // Process mesh indices
    std::cout << "GeometryEngine::processNode found " << ai_mesh->mNumFaces << " faces in current mesh." << std::endl;
    // std::cout << "Indices: " << std::endl;
    for(unsigned int j = 0; j < ai_mesh->mNumFaces; j++) {
        // std::cout << "Face: " << j << std::endl;;
        aiFace face = ai_mesh->mFaces[j];
        for(unsigned int k = 0; k < face.mNumIndices; k++) {
            // std::cout << face.mIndices[k] << std::endl;
            mesh->indices.push_back(face.mIndices[k]);
        }
    }

    // Process material
    if(ai_mesh->mMaterialIndex >= 0) {
        aiMaterial *material = scene->mMaterials[ai_mesh->mMaterialIndex];

        std::vector<QOpenGLTexture*> diffuse_maps = loadMaterialTextures(mesh, material, aiTextureType_DIFFUSE, "texture_diffuse");
        // std::vector<TextureData> specular_maps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

        mesh->textures.insert(mesh->textures.end(), diffuse_maps.begin(), diffuse_maps.end());
        // mesh->textures.insert(mesh->textures.end(), specular_maps.begin(), specular_maps.end());
    }

    //! [1]
    // Transfer vertex data to VBO 0
    mesh->array_buf.bind();
    mesh->array_buf.allocate(mesh->vertices.data(), mesh->vertices.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    mesh->index_buf.bind();
    mesh->index_buf.allocate(mesh->indices.data(), mesh->indices.size() * sizeof(GLuint));
    //! [1]

    mesh_.push_back(mesh);
}

std::vector<QOpenGLTexture*> GeometryEngine::loadMaterialTextures(Mesh* mesh, aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<QOpenGLTexture*> textures;
    std::cout << "GeometryEngine::loadMaterialTextures found " << mat->GetTextureCount(type) << " textures of type: " << type << std::endl;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        std::filesystem::path tex_path(directory_);
        tex_path /= str.C_Str();

        // std::cout << tex_path << std::endl;

        QImage img(tex_path.string().c_str());
        QOpenGLTexture* texture = new QOpenGLTexture(img);
        textures.push_back(texture);
    }

    return textures;
}

void GeometryEngine::drawModelGeometry(QOpenGLShaderProgram *program, QMatrix4x4& view_matrix)
{
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    program->setUniformValue("passtype", 0);

    // Draw each mesh
    for(Mesh* mesh : mesh_) {
        mesh->textures[0]->bind();

        // Tell OpenGL which VBOs to use
        mesh->array_buf.bind();
        mesh->index_buf.bind();

        // Offset for position
        quintptr offset = 0;

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = program->attributeLocation("a_position");
        program->enableAttributeArray(vertexLocation);
        program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for texture coordinate
        offset += sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
        int texcoordLocation = program->attributeLocation("a_texcoord");
        program->enableAttributeArray(texcoordLocation);
        program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

        // Draw cube geometry using indices from VBO 1
        glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
    }

    // Second pass
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    program->setUniformValue("passtype", 1);

    // Draw each mesh
    for(Mesh* mesh : mesh_) {
        mesh->textures[0]->bind();
        program->setUniformValue("texture", 0);

        // Tell OpenGL which VBOs to use
        mesh->array_buf.bind();
        mesh->index_buf.bind();

        // Offset for position
        quintptr offset = 0;

        // Tell OpenGL programmable pipeline how to locate vertex position data
        int vertexLocation = program->attributeLocation("a_position");
        program->enableAttributeArray(vertexLocation);
        program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

        // Offset for texture coordinate
        offset += sizeof(QVector3D);

        // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
        int texcoordLocation = program->attributeLocation("a_texcoord");
        program->enableAttributeArray(texcoordLocation);
        program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

        // Draw cube geometry using indices from VBO 1
        glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
    }
}
