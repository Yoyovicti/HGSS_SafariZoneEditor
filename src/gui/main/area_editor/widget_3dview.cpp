#include "widget_3dview.hpp"

#include "manager/locale_manager.hpp"
#include "manager/object_data_manager.hpp"

#include <QMouseEvent>
#include <QDateTime>

#include <cmath>
#include <iostream>

Widget3DView::Widget3DView(QWidget* parent) : QOpenGLWidget(parent), start_time_(QDateTime::currentMSecsSinceEpoch()) {
    setFixedSize(400, 400);
    area_model_ = nullptr;
    highlighted_model_ = nullptr;
}

Widget3DView::~Widget3DView() {
    // Make sure the context is current when deleting textures and buffers.
    makeCurrent();
    if(area_model_) delete area_model_;
    
    highlighted_model_ = nullptr;

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

void Widget3DView::mousePressEvent(QMouseEvent *e) {
#ifdef DEBUG
    // Save mouse press position
    mouse_press_pos_ = QVector2D(e->position());
#endif
}

void Widget3DView::mouseReleaseEvent(QMouseEvent *e) {
#ifdef DEBUG
    // Mouse release position - mouse press position
    QVector2D movement = QVector2D(e->position()) - mouse_press_pos_;
    QVector3D axis = QVector3D(movement.y(), movement.x(), 0.0).normalized();
    float speed = movement.length() / 100.0;

    // Calculate new rotation axis and speed
    rotation_axis_ = (rotation_axis_ * angular_speed_ + axis * speed).normalized();
    angular_speed_ += speed;
#endif
}

void Widget3DView::timerEvent(QTimerEvent *) {
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

void Widget3DView::initializeGL() {
    initializeOpenGLFunctions();

    // Clear background color with widget color (for illusion of transparency)
    QColor bg_color = palette().color(backgroundRole());
    glClearColor(bg_color.redF(), bg_color.blueF(), bg_color.greenF(), 1.0f);

    initShaders();

    // Offset viewport for centering model
    glViewport(0, 15, 400, 415);
}

void Widget3DView::initShaders() {
    // Base shader program
    if (!base_program_.addShaderFromSourceFile(QOpenGLShader::Vertex, "assets/shaders/vshader.glsl"))
        close();
    if (!base_program_.addShaderFromSourceFile(QOpenGLShader::Fragment, "assets/shaders/fshader.glsl"))
        close();
    if (!base_program_.link())
        close();
    if (!base_program_.bind())
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

void Widget3DView::resizeGL(int w, int h) {
    const float aspect_ratio = static_cast<float>(w) / (h < 1 ? 1 : h);
    const float z_near = 1.0f;
    const float z_far = 10.0f;
    const float fov = 45.0f;

    // Set perspective projection
    projection_.setToIdentity();
    projection_.perspective(fov, aspect_ratio, z_near, z_far);
}

void Widget3DView::paintGL() {
    const float elapsed_time = getElapsedTime();
    const QMatrix4x4 view_matrix = createViewMatrix();

    setupRenderingState();
    clearBuffers();

    drawArea(view_matrix);
    drawOtherObjects(view_matrix);

    if(!highlighted_model_)
        return;

    drawSelectedStencil(view_matrix);
    drawSelectedOutline(view_matrix, elapsed_time);
    drawSelectedObject(view_matrix);
}

const float Widget3DView::getElapsedTime() {
    double curr_time = QDateTime::currentMSecsSinceEpoch();
    const float elapsed_time = (curr_time - start_time_) / 1000;

    return elapsed_time;
}

QMatrix4x4 Widget3DView::createViewMatrix() const {
    QMatrix4x4 matrix;

    matrix.translate(0.0, 0.0, -3.0);
    matrix.rotate(rotation_);
    matrix.rotate(50, QVector3D(1.0, 0.0, 0.0));

    return matrix;
}

void Widget3DView::setupRenderingState() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(STENCIL_MASK);

#ifndef DEBUG
    glEnable(GL_CULL_FACE);
#endif
}

void Widget3DView::clearBuffers() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Widget3DView::drawArea(const QMatrix4x4& view_matrix) {
    // The area does not contribute to the outline stencil
    glStencilMask(0x00);

    base_program_.bind();
    base_program_.setUniformValue("mvp_matrix", projection_ * view_matrix);

    area_model_->drawModel(&base_program_, TexturePass::Opaque);
}

void Widget3DView::drawOtherObjects(const QMatrix4x4& view_matrix) {
    // Non selected objects do not contribute to the outline stencil
    glStencilMask(0x00);

    base_program_.bind();
    base_program_.setUniformValue("mvp_matrix", projection_ * view_matrix);

    for(Model* model : object_models_) {
        if(model == highlighted_model_)
            continue;

        model->drawModel(&base_program_, TexturePass::Opaque);
        model->drawModel(&base_program_, TexturePass::Transparent);
    }
}

void Widget3DView::drawSelectedStencil(const QMatrix4x4& view_matrix) {
    // Non selected objects do not contribute to the outline stencil
    glEnable(GL_DEPTH_TEST);

    // Record object silhouettes in the stencil buffer
    glStencilFunc(GL_ALWAYS, STENCIL_OBJECT, STENCIL_MASK);
    glStencilMask(STENCIL_MASK);

    base_program_.bind();
    base_program_.setUniformValue("mvp_matrix", projection_ * view_matrix);

    highlighted_model_->drawModel(&base_program_, TexturePass::Opaque);
    highlighted_model_->drawModel(&base_program_, TexturePass::Transparent);
}

void Widget3DView::drawSelectedOutline(const QMatrix4x4& view_matrix, float elapsed_time) {
    // Only render where the stencil wasn't written
    glStencilFunc(GL_NOTEQUAL, STENCIL_OBJECT, STENCIL_MASK);

    // Stencil is only used as a mask during this pass
    glStencilMask(0x00);

    // Draw selected outline in front of previously drawn objects
    glDisable(GL_DEPTH_TEST);

    outline_program_.bind();
    outline_program_.setUniformValue("time", elapsed_time);

    const QMatrix4x4 outline_matrix = createOutlineMatrix(highlighted_model_, view_matrix);
    outline_program_.setUniformValue("mvp_matrix", projection_ * outline_matrix);

    highlighted_model_->drawModel(&outline_program_, TexturePass::Transparent);
}

QMatrix4x4 Widget3DView::createOutlineMatrix(const Model* model, const QMatrix4x4& view_matrix) const {
    const QVector3D center = model->boundingBoxCenter();

    // Scale around model center
    QMatrix4x4 matrix = view_matrix;
    matrix.translate(center);
    matrix.scale(OUTLINE_SCALE);
    matrix.translate(-center);

    return matrix;
}

void Widget3DView::drawSelectedObject(const QMatrix4x4& view_matrix) {
    // Stencil is not used during this pass
    glStencilMask(0x00);

    // Draw the selected object on top of everything else
    glDisable(GL_DEPTH_TEST);

    base_program_.bind();
    base_program_.setUniformValue("mvp_matrix", projection_ * view_matrix);

    highlighted_model_->drawModel(&base_program_, TexturePass::Opaque);
    highlighted_model_->drawModel(&base_program_, TexturePass::Transparent);
}

void Widget3DView::startHighlightModel(uint8_t i) {
    highlighted_model_ = object_models_[i];
    update();
}

void Widget3DView::stopHighlightModel(uint8_t i) {
    highlighted_model_ = nullptr;
    update();
}
