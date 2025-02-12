#ifndef MESH_HPP
#define MESH_HPP

#include <assimp/material.h>
#include <assimp/mesh.h>

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShader>
#include <QOpenGLTexture>
#include <QVector3D>

#include <filesystem>

struct BBox {
    QVector3D min_;
    QVector3D max_;
};

struct VertexData {
    QVector3D position_;
    // QVector3D normal_;
    QVector2D tex_coords_;
};

class Mesh : protected QOpenGLFunctions {
public:
    Mesh(const aiMaterial* material, const aiMesh* mesh, const std::filesystem::path& model_dir, const QVector3D& xyz_offset, const QVector3D& scale, BBox& bbox);

    ~Mesh() {
        // Delete textures
        for(QOpenGLTexture* texture : textures_) {
            delete texture;
        }

        // Destroy VBOs
        array_buf_.destroy();
        index_buf_.destroy();
    }

    void drawMesh(QOpenGLShaderProgram* program);

private:
    std::vector<VertexData> vertices_;
    std::vector<GLuint> indices_;
    std::vector<QOpenGLTexture*> textures_;

    QOpenGLBuffer index_buf_;
    QOpenGLBuffer array_buf_;

    void processVertices(const aiMesh* mesh, const QVector3D& xyz_offset, const QVector3D& scale, BBox& bbox);
    void processIndices(const aiMesh* mesh);
    void processMaterial(const aiMaterial* material, const aiMesh* mesh, const std::filesystem::path& model_dir);
    std::vector<QOpenGLTexture*> loadMaterialTextures(const aiMaterial *material, const std::filesystem::path& model_dir, aiTextureType type, std::string type_name);
};

#endif // MESH_HPP
