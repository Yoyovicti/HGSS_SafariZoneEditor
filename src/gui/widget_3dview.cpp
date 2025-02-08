#include "gui/widget_3dview.hpp"

#include <QMouseEvent>

#include <cmath>
#include <iostream>

Widget3DView::Widget3DView(QWidget* parent) : QOpenGLWidget(parent) {
    setFixedSize(400, 400);
    area_model_ = nullptr;
}

Widget3DView::~Widget3DView()
{
    // Make sure the context is current when deleting textures and buffers.
    makeCurrent();
    if(area_model_) delete area_model_;
    doneCurrent();
}

void Widget3DView::setModelDir(const std::filesystem::path& model_dir) {
    model_dir_ = model_dir;

    makeCurrent();
    if(area_model_) delete area_model_;
    area_model_ = new Model(model_dir_);
    doneCurrent();
    update();
}

// void Widget3DView::setSlot(const Slot& slot) {
//     slot_ = slot;

//     // TODO fix geometries are drawn 2 times
//     if(geometries_) {
//         makeCurrent();
//         delete geometries_;
//         geometries_ = new GeometryEngine();
//         geometries_->setObjectDirectory(model_dir_);
//         doneCurrent();

//         update();
//     }
// }

void Widget3DView::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mouse_press_pos_ = QVector2D(e->position());
}

void Widget3DView::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->position()) - mouse_press_pos_;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotation_axis_ = (rotation_axis_ * angular_speed_ + n * acc).normalized();

    // Increase angular speed
    angular_speed_ += acc;
}

void Widget3DView::timerEvent(QTimerEvent *)
{
#ifdef DEBUG
    // Decrease angular speed (friction)
    angular_speed_ *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angular_speed_ < 0.01) {
        angular_speed_ = 0.0;
    } else {
        // Update rotation
        rotation_ = QQuaternion::fromAxisAndAngle(rotation_axis_, angular_speed_) * rotation_;

        // Request an update
        update();
    }
#endif
}

void Widget3DView::initializeGL()
{
    initializeOpenGLFunctions();

    // Clear background color with widget color (for illusion of transparency)
    QColor bg_color = palette().color(backgroundRole());
    glClearColor(bg_color.redF(), bg_color.blueF(), bg_color.greenF(), 1.0f);

    initShaders();

#ifdef DEBUG
    timer_.start(12, this);
#endif
}

void Widget3DView::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, "assets/shaders/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, "assets/shaders/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}

void Widget3DView::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane, far plane and FOV
    const qreal zNear = 1.0, zFar = 10.0, fov = 45.0;

    // Reset projection
    projection_.setToIdentity();

    // Set perspective projection
    projection_.perspective(fov, aspect, zNear, zFar);
}

void Widget3DView::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Offset viewport for centering model
    glViewport(0, 15, 400, 415);

#ifndef DEBUG
    // Enable back face culling
    glEnable(GL_CULL_FACE);
#endif

    program.bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -3.0);
    matrix.rotate(rotation_);
    matrix.rotate(50, QVector3D(1.0, 0.0, 0.0));

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection_ * matrix);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Draw model in 2 passes : first with only opaque fragments, second with only transparent fragments
    area_model_->drawModel(&program, 0);
    area_model_->drawModel(&program, 1);
}
