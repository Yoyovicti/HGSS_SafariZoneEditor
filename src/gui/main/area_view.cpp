#include "area_view.hpp"

#include <QScrollBar>
#include <QKeyEvent>

AreaView::AreaView(SaveDataManager* save_data_manager, QWidget *parent) : QWidget(parent), save_data_manager_(save_data_manager), layout_(this), back_button_(this), obj_area_(this), object_view_(this), objects_label_(this), view_3d_(this), selected_slot_(-1), selected_object_(-1), edit_mode_(false) {
    back_button_.setText("< Retour");
    day_label_.setText("Jours");

    objects_label_.setText("Objets");

    // Disable horizontal scrollbar
    obj_area_.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    obj_area_.horizontalScrollBar()->setEnabled(false);

    obj_area_.setWidget(&object_view_);

    layout_.addWidget(&back_button_, 0, 0, 1, 1);

    layout_.addWidget(&day_label_, 0, 3, 1, 1);
    layout_.addWidget(&day_edit_, 0, 4, 1, 1);

    layout_.addWidget(&objects_label_, 0, 5, 1, 1);
    layout_.addWidget(&obj_add_button_, 0, 6, 1, 1);

    layout_.addWidget(&view_3d_, 1, 0, 5, 5);

    layout_.addWidget(&obj_area_, 1, 5, 5, 2);

    QObject::connect(&back_button_, &QPushButton::released, this, [this](){emit backButtonReleased();});
    QObject::connect(&object_view_, &ObjectView::enterItemHover, this, &AreaView::enterItemHover);
    QObject::connect(&object_view_, &ObjectView::leaveItemHover, this, &AreaView::leaveItemHover);
    QObject::connect(&object_view_, &ObjectView::moveButtonClicked, &view_3d_, [this](uint8_t i){
        edit_mode_ = !edit_mode_;
        selected_object_ = i;
    });
}

void AreaView::setSlot(uint8_t slot_index) {
    const Slot& slot = save_data_manager_->getSlots()[slot_index];
    QObject::connect(&day_edit_, &DayCounterEdit::dayCountChanged, this, [this, slot](uint8_t count) {
        emit counterChanged(slot.area_type_, count);
    });

    selected_slot_ = slot_index;
    object_view_.setSlot(slot);
    view_3d_.setObjects(slot);
}

void AreaView::keyReleaseEvent(QKeyEvent* event) {
    QWidget::keyReleaseEvent(event);  // Call base class implementation if needed
    if(!edit_mode_) return;

    QVector3D move;
    switch(event->key()) {
    case Qt::Key_S:
        move = {0, 0, 1};
        break;
    case Qt::Key_W:
        move = {0, 0, -1};
        break;
    case Qt::Key_A:
        move = {-1, 0, 0};
        break;
    case Qt::Key_D:
        move = {1, 0, 0};
        break;
    }

    save_data_manager_->translateObject(selected_slot_, selected_object_, move);
    view_3d_.translateObject(selected_object_, move);
    object_view_.updateObjectPosition(selected_object_);
}

void AreaView::enterItemHover(uint8_t i) {
    view_3d_.startHighlightModel(i);
}

void AreaView::leaveItemHover(uint8_t i) {
    view_3d_.stopHighlightModel(i);
}
