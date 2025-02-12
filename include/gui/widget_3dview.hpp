#ifndef WIDGET_3DVIEW_HPP
#define WIDGET_3DVIEW_HPP

#include "manager/save_data_manager.hpp"
#include "model.hpp"

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include <filesystem>

#define DEBUG

class Widget3DView : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    Widget3DView(QWidget* parent = nullptr);
    ~Widget3DView();

    void setModelDir(const std::filesystem::path& model_dir);
    void setObjects(const Slot& slot);

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void timerEvent(QTimerEvent *e) override;

    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void initShaders();

private:
    const std::string JSON_KEY = "objects";

    std::filesystem::path model_dir_;

#ifdef DEBUG
    QBasicTimer timer_;
#endif
    QOpenGLShaderProgram program_;
    QOpenGLShaderProgram outline_program_;

    Model* area_model_;
    std::vector<Model*> object_models_;

    QMatrix4x4 projection_;
    QVector2D mouse_press_pos_;
    QVector3D rotation_axis_;
    qreal angular_speed_ = 0;
    QQuaternion rotation_;

    Slot slot_;
};

#endif // WIDGET_3DVIEW_HPP

