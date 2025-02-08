#ifndef GEOMETRYENGINE_H
#define GEOMETRYENGINE_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include <filesystem>
#include <vector>
#include <string>

struct VertexDataOld {
    QVector3D position;
    QVector2D tex_coords;
};

struct MeshOld {
    MeshOld() : index_buf(QOpenGLBuffer::IndexBuffer) {
        // Generate VBOs
        array_buf.create();
        index_buf.create();
    }

    ~MeshOld() {
        // Destroy VBOs
        array_buf.destroy();
        index_buf.destroy();
    }

    std::vector<VertexDataOld> vertices;
    std::vector<GLuint> indices;
    std::vector<QOpenGLTexture*> textures;

    QOpenGLBuffer index_buf;
    QOpenGLBuffer array_buf;
};

class GeometryEngine : protected QOpenGLFunctions
{
public:
    GeometryEngine();
    virtual ~GeometryEngine();

    void setModelDirectory(std::filesystem::path& path);
    void drawModelGeometry(QOpenGLShaderProgram *program/*, QMatrix4x4& view_matrix*/);

private:
    void initModelGeometry();
    void initObjectGeometry();

    void processNode(aiNode* node, const aiScene* scene);
    void processObjNode(aiNode* node, const aiScene* scene, const std::filesystem::path& tex_dir, const QVector3D& coords);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    void processObjMesh(aiMesh* mesh, const aiScene* scene, const std::filesystem::path& tex_dir, const QVector3D& coords);
    std::vector<QOpenGLTexture*> loadMaterialTextures(MeshOld* mesh, aiMaterial *mat, aiTextureType type, std::string typeName);
    std::vector<QOpenGLTexture*> loadObjMaterialTextures(MeshOld* mesh, aiMaterial *mat, aiTextureType type, std::string typeName, const std::filesystem::path& tex_dir);

    void initTexture();

    std::filesystem::path directory_;

    // Meshes are stored here
    std::vector<MeshOld*> mesh_;
};

#endif // GEOMETRYENGINE_H
