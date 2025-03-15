#include "render/mesh.hpp"

#include <limits>

Mesh::Mesh(const aiMaterial* material, const aiMesh* mesh, const std::filesystem::path& model_dir, const QVector3D& xyz_offset, const QVector3D& scale, BBox& bbox) : index_buf_(QOpenGLBuffer::IndexBuffer) {
    initializeOpenGLFunctions();

    // Generate VBOs
    array_buf_.create();
    index_buf_.create();

    processVertices(mesh, xyz_offset, scale, bbox);
    processIndices(mesh);
    processMaterial(material, mesh, model_dir);

    // Transfer vertex data to VBO 0
    array_buf_.bind();
    array_buf_.allocate(vertices_.data(), vertices_.size() * sizeof(VertexData));

    // Transfer index data to VBO 1
    index_buf_.bind();
    index_buf_.allocate(indices_.data(), indices_.size() * sizeof(GLuint));
}

void Mesh::processVertices(const aiMesh* mesh, const QVector3D& xyz_offset, const QVector3D& scale, BBox& bbox) {


    for(size_t i = 0; i < mesh->mNumVertices; i++) {
        if(mesh->mVertices[i].x < bbox.min_.x()) bbox.min_.setX(mesh->mVertices[i].x);
        if(mesh->mVertices[i].y < bbox.min_.y()) bbox.min_.setY(mesh->mVertices[i].y);
        if(mesh->mVertices[i].z < bbox.min_.z()) bbox.min_.setZ(mesh->mVertices[i].z);
        if(mesh->mVertices[i].x > bbox.max_.x()) bbox.max_.setX(mesh->mVertices[i].x);
        if(mesh->mVertices[i].y > bbox.max_.y()) bbox.max_.setY(mesh->mVertices[i].y);
        if(mesh->mVertices[i].z > bbox.max_.z()) bbox.max_.setZ(mesh->mVertices[i].z);

        // Vertex position
        QVector3D position(
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        );
        position += xyz_offset;
        position *= scale;


        // // Normal
        // QVector3D normal(
        //     mesh->mNormals[i].x,
        //     mesh->mNormals[i].y,
        //     mesh->mNormals[i].z
        // );

        // Texture coordinates
        QVector2D tex_coords(0.0, 0.0);
        if(mesh->mTextureCoords[0]) {
            tex_coords = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };
        }

        VertexData v_data;
        v_data.position_ = position;
        // v_data.normal_ = normal;
        v_data.tex_coords_ = tex_coords;
        vertices_.push_back(v_data);
    }
}

void Mesh::processIndices(const aiMesh* mesh) {
    for(size_t i = 0; i < mesh->mNumFaces; i++) {
        const aiFace& face = mesh->mFaces[i];
        for(size_t j = 0; j < face.mNumIndices; j++) {
            indices_.push_back(face.mIndices[j]);
        }
    }
}

void Mesh::processMaterial(const aiMaterial* material, const aiMesh* mesh, const std::filesystem::path& model_dir) {
    if(mesh->mMaterialIndex >= 0) {
        std::vector<QOpenGLTexture*> diffuse_maps = loadMaterialTextures(material, model_dir, aiTextureType_DIFFUSE, "texture_diffuse");
        textures_.insert(textures_.end(), diffuse_maps.begin(), diffuse_maps.end());
    }
}

std::vector<QOpenGLTexture*> Mesh::loadMaterialTextures(const aiMaterial *material, const std::filesystem::path& model_dir, aiTextureType type, std::string type_name) {
    // TODO refactor this part
    std::vector<QOpenGLTexture*> textures;
    for(size_t i = 0; i < material->GetTextureCount(type); i++) {
        aiString str;
        material->GetTexture(type, i, &str);

        std::filesystem::path tex_path(model_dir / str.C_Str());
        QImage img(tex_path.string().c_str());
        QOpenGLTexture* texture = new QOpenGLTexture(img);
        textures.push_back(texture);
    }

    return textures;
}

void Mesh::drawMesh(QOpenGLShaderProgram* program) {
    // We only use first texture
    textures_[0]->bind();

    // Tell OpenGL which VBOs to use
    array_buf_.bind();
    index_buf_.bind();

    // Offset for vertex data
    quintptr offset = 0;

    // Locate vertex position data
    int vertex_location = program->attributeLocation("a_position");
    program->enableAttributeArray(vertex_location);
    program->setAttributeBuffer(vertex_location, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Update offset
    offset += sizeof(QVector3D);

    // // Locate normal data
    // int normal_location = program->attributeLocation("a_normal");
    // program->enableAttributeArray(normal_location);
    // program->setAttributeBuffer(normal_location, GL_FLOAT, offset, 3, sizeof(VertexData));

    // // Update offset
    // offset += sizeof(QVector3D);

    // Locate vertex tex coords data
    int tex_coords_location = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(tex_coords_location);
    program->setAttributeBuffer(tex_coords_location, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw mesh using indices
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, nullptr);
}
