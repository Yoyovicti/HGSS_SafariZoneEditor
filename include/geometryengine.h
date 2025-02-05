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

struct VertexData {
    QVector3D position;
    QVector2D tex_coords;
};

struct Mesh {
    Mesh() : index_buf(QOpenGLBuffer::IndexBuffer) {
        // Generate VBOs
        array_buf.create();
        index_buf.create();
    }

    ~Mesh() {
        // Destroy VBOs
        array_buf.destroy();
        index_buf.destroy();
    }

    std::vector<VertexData> vertices;
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

    void setDirectory(std::filesystem::path& path);
    void drawModelGeometry(QOpenGLShaderProgram *program, QMatrix4x4& view_matrix);

private:
    void initModelGeometry();
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<QOpenGLTexture*> loadMaterialTextures(Mesh* mesh, aiMaterial *mat, aiTextureType type, std::string typeName);

    void initTexture();

    std::filesystem::path directory_;

    // Meshes are stored here
    std::vector<Mesh*> mesh_;
};

#endif // GEOMETRYENGINE_H
