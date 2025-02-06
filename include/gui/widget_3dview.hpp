#ifndef WIDGET_3DVIEW_HPP
#define WIDGET_3DVIEW_HPP

#include "geometryengine.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <filesystem>

class Widget3DView : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    // using QOpenGLWidget::QOpenGLWidget;
    Widget3DView(/*std::filesystem::path& model_path, */QWidget* parent = nullptr);
    ~Widget3DView();

    void setModelPath(std::filesystem::path& model_path);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();
    // void initTextures();

private:
    std::filesystem::path model_dir_;

#ifdef DEBUG
    QBasicTimer timer;
#endif
    QOpenGLShaderProgram program;
    GeometryEngine *geometries = nullptr;

    // QOpenGLTexture *texture = nullptr;

    QMatrix4x4 projection;

    QVector2D mousePressPosition;
    QVector3D rotationAxis;
    qreal angularSpeed = 0;
    QQuaternion rotation;
};

#endif // WIDGET_3DVIEW_HPP

