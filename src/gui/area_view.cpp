#include "gui/area_view.hpp"

AreaView::AreaView(QWidget *parent) : QWidget(parent), layout_(this), back_button_(this), view_3d_(this) {

    back_button_.setText("< Retour");
    // back_button_.set

    layout_.addWidget(&back_button_, 0, 0, 1, 1);
    layout_.addWidget(&view_3d_, 1, 0, 5, 5);

    QObject::connect(&back_button_, &QPushButton::released, this, [&](){emit backButtonReleased();});
}
