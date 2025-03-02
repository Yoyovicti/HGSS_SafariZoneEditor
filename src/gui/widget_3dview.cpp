#include "gui/widget_3dview.hpp"

#include "manager/locale_manager.hpp"
#include "manager/object_data_manager.hpp"

#include <QMouseEvent>
#include <QDateTime>

#include <cmath>
#include <iostream>

Widget3DView::Widget3DView(QWidget* parent) : QOpenGLWidget(parent), start_time_(QDateTime::currentMSecsSinceEpoch()) {
    setFixedSize(400, 400);
    area_model_ = nullptr;
}

Widget3DView::~Widget3DView()
{
    // Make sure the context is current when deleting textures and buffers.
    makeCurrent();
    if(area_model_) delete area_model_;
    for(Model* model : object_models_) {
        if(model) delete model;
    }
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

void Widget3DView::setObjects(const Slot& slot) {
    makeCurrent();
    for(Model* model : object_models_) {
        if(model) delete model;
    }
    object_models_.clear();

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json object_table;
    if(!locale_manager.getTable(&object_table, JSON_KEY)) {
        std::cerr << "Unable to load blocks table" << std::endl;
        return;
    }

    ObjectDataManager& obj_manager = ObjectDataManager::getInstance();

    const std::filesystem::path global_obj_dir("assets/objects/models");
    for(uint8_t i = 0; i < slot.object_count_; i++) {
        Object obj = slot.objects_[i];
        std::string en_name = object_table[obj.id_][0];
        std::filesystem::path obj_dir(global_obj_dir / en_name);

        json obj_data_table;
        if(!obj_manager.getTable(&obj_data_table, en_name)) {
            std::cerr << "Unknown object data key: " << en_name << std::endl;
            continue;
        }
        uint8_t width = obj_data_table["width"];
        uint8_t height = obj_data_table["height"];

        QVector3D offset(
            16.0f * (obj.x_ - (16.0f - float(width) * 0.5f)),
            obj.y_,
            16.0f * (obj.z_ - (15.0f + float(height) * 0.5f))
        );

        Model* model = new Model(obj_dir, offset);
        object_models_.push_back(model);
    }
}

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
    }
#endif

    // Request an update
    update();
}

void Widget3DView::initializeGL()
{
    initializeOpenGLFunctions();

    // Clear background color with widget color (for illusion of transparency)
    QColor bg_color = palette().color(backgroundRole());
    glClearColor(bg_color.redF(), bg_color.blueF(), bg_color.greenF(), 1.0f);

    initShaders();

    timer_.start(12, this);

    // Offset viewport for centering model
    glViewport(0, 15, 400, 415);
}

void Widget3DView::initShaders()
{
    // Base shader program
    // Compile vertex shader
    if (!program_.addShaderFromSourceFile(QOpenGLShader::Vertex, "assets/shaders/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program_.addShaderFromSourceFile(QOpenGLShader::Fragment, "assets/shaders/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program_.link())
        close();

    // Bind shader pipeline for use
    if (!program_.bind())
        close();

    // Outline program
    if (!outline_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, "assets/shaders/vshader.glsl"))
        close();
    if(!outline_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, "assets/shaders/foutline.glsl"))
        close();
    if(!outline_program_.link())
        close();
    if(!outline_program_.bind())
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
    double curr_time = QDateTime::currentMSecsSinceEpoch();
    float elapsed_time = (curr_time - start_time_) / 1000;

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Stencil test
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

#ifndef DEBUG
    // Enable back face culling
    glEnable(GL_CULL_FACE);
#endif

    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    outline_program_.bind();

    // Calculate model view transformation
    QMatrix4x4 matrix;
    matrix.translate(0.0, 0.0, -3.0);
    matrix.rotate(rotation_);
    matrix.rotate(50, QVector3D(1.0, 0.0, 0.0));

    // Set modelview-projection matrix
    outline_program_.setUniformValue("mvp_matrix", projection_ * matrix);

    program_.bind();
    program_.setUniformValue("mvp_matrix", projection_ * matrix);

    glStencilMask(0x00);

    // draw non-outlined models (aka floor), do not write stencil buffer
    // Draw model in 2 passes : first with only opaque fragments, second with only transparent fragments
    area_model_->drawModel(&program_, 0);
    // area_model_->drawModel(&program, 1);

    // First pass, draw objects as normal and write stencil buffer
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    for(Model* model : object_models_) {
        model->drawModel(&program_, 0);
        // model->drawModel(&program, 1);
    }

    // Second pass : Draw scaled version of objects, do not write stencil buffer
    // Stencil buffer is filled with 1's from objects, which are not drawn this time.
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    outline_program_.bind();
    outline_program_.setUniformValue("time", elapsed_time);
    float scale = 2.0f;

    // TODO fix this mess: Compute bounding box of model and retrieve center, then translate to origin, then translate back
    for(Model* model : object_models_) {
        QVector3D centroid(
            (model->bbox_.max_.x() + model->bbox_.min_.x()) / 2,
            (model->bbox_.max_.y() + model->bbox_.min_.y()) / 2,
            (model->bbox_.max_.z() + model->bbox_.min_.z()) / 2
        );
        
        // Calculate model view transformation
        QMatrix4x4 matrix2;

        matrix2.translate(-centroid);
        matrix2.scale(scale);
        matrix2.translate(centroid);
        matrix2.translate(0.0, 0.0, -3.0);
        matrix2.rotate(rotation_);
        matrix2.rotate(50, QVector3D(1.0, 0.0, 0.0));

        // Set modelview-projection matrix
        outline_program_.setUniformValue("mvp_matrix", projection_ * matrix2);

        model->drawModel(&outline_program_, 0);
        // model->drawModel(&program, 1);
    }

    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);

}

void Widget3DView::startHighlightModel(uint8_t i) {
    object_models_[i]->setHighlight(true);
    update();
}

void Widget3DView::stopHighlightModel(uint8_t i) {
    object_models_[i]->setHighlight(false);
    update();
}
