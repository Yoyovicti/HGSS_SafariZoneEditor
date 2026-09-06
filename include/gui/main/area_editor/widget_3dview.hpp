#ifndef WIDGET_3DVIEW_HPP
#define WIDGET_3DVIEW_HPP

#include "manager/save_data_manager.hpp"
#include "render/model.hpp"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <filesystem>

constexpr float OUTLINE_SCALE = 1.4f;

constexpr GLint STENCIL_OBJECT = 1;
constexpr GLuint STENCIL_MASK = 0xFF;

class Widget3DView : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    Widget3DView(QWidget* parent = nullptr);
    ~Widget3DView();

    void setModelDir(const std::filesystem::path& model_dir);
    void setObjects(const Slot& slot);

private:
    const std::string JSON_KEY = "objects";

    std::filesystem::path model_dir_;

    QOpenGLShaderProgram base_program_;
    QOpenGLShaderProgram outline_program_;

    Model* area_model_;
    Model* highlighted_model_;
    std::vector<Model*> object_models_;

    QMatrix4x4 projection_;
    QQuaternion rotation_;

#ifdef DEBUG
    QVector2D mouse_press_pos_;
    QVector3D rotation_axis_;
    qreal angular_speed_ = 0;
#endif

    Slot slot_;
    double start_time_;

    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void initShaders();

    void resizeGL(int w, int h) override;

    void paintGL() override;
    const float getElapsedTime();
    QMatrix4x4 createViewMatrix() const;
    void setupRenderingState();
    void clearBuffers();
    void drawArea(const QMatrix4x4& view_matrix);
    void drawOtherObjects(const QMatrix4x4& view_matrix);
    void drawSelectedStencil(const QMatrix4x4& view_matrix);
    void drawSelectedOutline(const QMatrix4x4& view_matrix, float elapsed_time);
    QMatrix4x4 createOutlineMatrix(const Model* Model, const QMatrix4x4& view_matrix) const;
    void drawSelectedObject(const QMatrix4x4& view_matrix);

public slots:
    void startHighlightModel(uint8_t i);
    void stopHighlightModel(uint8_t i);
};

#endif // WIDGET_3DVIEW_HPP

