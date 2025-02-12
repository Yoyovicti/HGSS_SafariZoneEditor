#include "model.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>

Model::Model(const std::filesystem::path& model_dir, const QVector3D& xyz_offset) : model_dir_(model_dir), xyz_offset_(xyz_offset) {
    initializeOpenGLFunctions();

    bbox_.max_ = std::numeric_limits<QVector3D>::min();
    bbox_.min_ = std::numeric_limits<QVector3D>::max();
    bool model_found = false;

    std::filesystem::path model_path;
    for(const auto& entry : std::filesystem::directory_iterator(model_dir)) {
        if(entry.path().extension() == ".dae") {
            model_path = entry.path();
            model_found = true;
            break;
        }
    }

    if(!model_found) {
        std::cerr << "Model::Model unable to find COLLADA .dae file at: " << model_dir << std::endl;
        return;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(model_path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Model::Model unable to load Assimp model: " << importer.GetErrorString() << std::endl;
        return;
    }

    processNode(scene->mRootNode, scene);

    bbox_.max_ /= 256;
    bbox_.min_ /= 256;
}

// https://learnopengl.com/Model-Loading/Assimp
void Model::processNode(const aiNode* node, const aiScene* scene) {
    // Process each mesh in current node
    for(size_t i = 0; i < node->mNumMeshes; i++) {
        aiMesh* ai_mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(ai_mesh, scene);
    }

    // Process each child node
    for(size_t i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void Model::processMesh(const aiMesh* ai_mesh, const aiScene* scene) {
    // Don't process meshes under the surface
    if(ai_mesh->mNumVertices > 0 && ai_mesh->mVertices[0].y < -8.0f)
        return;

    aiMaterial* material = scene->mMaterials[ai_mesh->mMaterialIndex];
    QVector3D scale(1.0, 1.0, 1.0);
    Mesh* mesh = new Mesh(material, ai_mesh, model_dir_, xyz_offset_, scale / 256, bbox_);
    meshes_.push_back(mesh);
}

void Model::drawModel(QOpenGLShaderProgram* program, uint8_t pass_type) {
    switch(pass_type) {
    case 0:
        glDisable(GL_BLEND);
        break;

    case 1:
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;

    default:
        std::cerr << "Model::drawModel unknown pass_type: " << int(pass_type) << std::endl;
        return;
    }

    program->setUniformValue("pass_type", pass_type);
    for(Mesh* mesh : meshes_) {
        mesh->drawMesh(program);
    }
}


