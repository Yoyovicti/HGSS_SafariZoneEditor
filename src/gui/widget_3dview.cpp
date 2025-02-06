#include "gui/widget_3dview.hpp"

#include <QMouseEvent>

#include <cmath>
#include <iostream>

Widget3DView::Widget3DView(QWidget* parent) : QOpenGLWidget(parent) {
    setFixedSize(400, 400);
    model_dir_ = "assets/maps/Forest";
}

Widget3DView::~Widget3DView()
{
    // Make sure the context is current when deleting textures and buffers.
    makeCurrent();
    delete geometries;
    doneCurrent();
}

void Widget3DView::setModelPath(std::filesystem::path& model_dir)
{
    model_dir_ = model_dir;
}

//! [0]
void Widget3DView::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->position());
}

void Widget3DView::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    QVector2D diff = QVector2D(e->position()) - mousePressPosition;

    // Rotation axis is perpendicular to the mouse position difference
    // vector
    QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

    // Accelerate angular speed relative to the length of the mouse sweep
    qreal acc = diff.length() / 100.0;

    // Calculate new rotation axis as weighted sum
    rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

    // Increase angular speed
    angularSpeed += acc;
}
//! [0]

//! [1]
void Widget3DView::timerEvent(QTimerEvent *)
{
#ifdef DEBUG
    // Decrease angular speed (friction)
    angularSpeed *= 0.99;

    // Stop rotation when speed goes below threshold
    if (angularSpeed < 0.01) {
        angularSpeed = 0.0;
    } else {
        // Update rotation
        rotation = QQuaternion::fromAxisAndAngle(rotationAxis, angularSpeed) * rotation;

        // Request an update
        update();
    }
#endif
}
//! [1]

void Widget3DView::initializeGL()
{
    initializeOpenGLFunctions();

    QColor bg_color = palette().color(backgroundRole());
    glClearColor(bg_color.redF(), bg_color.blueF(), bg_color.greenF(), 1.0f);

    initShaders();

    geometries = new GeometryEngine();
    geometries->setDirectory(model_dir_);

#ifdef DEBUG
    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
#endif
}

//! [3]
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

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 10.0, fov = 50.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}

void Widget3DView::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Enable back face culling
#ifndef DEBUG
    glEnable(GL_CULL_FACE);
#endif

    program.bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -3.0);
    matrix.rotate(rotation);
    matrix.rotate(45, QVector3D(1.0, 0.0, 0.0));

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);

    // Use texture unit 0 which contains cube.png

    // Draw model geometry
    geometries->drawModelGeometry(&program, matrix);
}
