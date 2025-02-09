#include "gui/area_view.hpp"

AreaView::AreaView(QWidget *parent) : QWidget(parent), layout_(this), back_button_(this), object_view_(this), objects_label_(this), view_3d_(this), obj_area_(this) {

    back_button_.setText("< Retour");
    objects_label_.setText("Objets");

    obj_area_.setWidget(&object_view_);

    layout_.addWidget(&back_button_, 0, 0, 1, 1);
    layout_.addWidget(&objects_label_, 0, 5, 1, 1);
    layout_.addWidget(&view_3d_, 1, 0, 5, 5);
    layout_.addWidget(&obj_area_, 1, 5, 5, 1);



    QObject::connect(&back_button_, &QPushButton::released, this, [&](){emit backButtonReleased();});
}

void AreaView::setSlot(const Slot& slot) {
    object_view_.setSlot(slot);
    view_3d_.setObjects(slot);
}
