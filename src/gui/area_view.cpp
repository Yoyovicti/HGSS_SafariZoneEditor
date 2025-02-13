#include "gui/area_view.hpp"

#include <QScrollBar>

AreaView::AreaView(QWidget *parent) : QWidget(parent), layout_(this), back_button_(this), obj_area_(this), object_view_(this), objects_label_(this), view_3d_(this) {

    back_button_.setText("< Retour");
    objects_label_.setText("Objets");

    obj_area_.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    obj_area_.horizontalScrollBar()->setEnabled(false);
    // obj_area_.setWidgetResizable(true);
    // obj_area_.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

    // QSizePolicy policy = obj_area_.sizePolicy();
    // policy.setHorizontalStretch(1);
    // policy.setVerticalStretch(1);

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

    // object_view_.adjustSize();
    obj_area_.setFixedWidth(object_view_.getWidth() + obj_area_.verticalScrollBar()->sizeHint().width() + 20);

    // obj_area_.setFixedWidth(object_view_.width() + obj_area_.verticalScrollBar()->width());
}
