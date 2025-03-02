#include "gui/area_view.hpp"

#include <QScrollBar>

AreaView::AreaView(QWidget *parent) : QWidget(parent), layout_(this), back_button_(this), obj_area_(this), object_view_(this), objects_label_(this), view_3d_(this) {

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

    QObject::connect(&back_button_, &QPushButton::released, this, [&](){emit backButtonReleased();});
    QObject::connect(&object_view_, &ObjectView::enterItemHover, &view_3d_, &Widget3DView::startHighlightModel);
    QObject::connect(&object_view_, &ObjectView::leaveItemHover, &view_3d_, &Widget3DView::stopHighlightModel);
}

void AreaView::setSlot(const Slot& slot) {

    QObject::connect(&day_edit_, &DayCounterEdit::dayCountChanged, this, [&](uint8_t count) {
        emit counterChanged(slot.area_type_, count);
    });

    object_view_.setSlot(slot);
    view_3d_.setObjects(slot);
}
