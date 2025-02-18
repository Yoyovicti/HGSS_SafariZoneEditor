#ifndef MODEL_HPP
#define MODEL_HPP

#include "mesh.hpp"

#include <assimp/scene.h>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector3D>

#include <filesystem>

class Model : protected QOpenGLFunctions {
public:
    Model(const std::filesystem::path& model_dir, const QVector3D& xyz_offset);
    Model(const std::filesystem::path& model_dir) : Model::Model(model_dir, QVector3D(0.0, 0.0, 0.0)) {}

    ~Model() {
        for(Mesh* mesh : meshes_) {
            delete mesh;
        }
    }

    void drawModel(QOpenGLShaderProgram* program, uint8_t pass_type);

    void setHighlight(bool highlight = true) {highlight_ = highlight;}

    BBox bbox_;

private:
    std::vector<Mesh*> meshes_;
    std::filesystem::path model_dir_;

    QVector3D xyz_offset_;
    bool highlight_;

    void processNode(const aiNode* node, const aiScene* scene);
    void processMesh(const aiMesh* ai_mesh, const aiScene* scene);
};

#endif // MODEL_HPP
