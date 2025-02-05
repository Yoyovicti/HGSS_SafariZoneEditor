#include "gui/area_label.hpp"

AreaLabel::AreaLabel(QWidget* parent) : QLabel(parent) {
    setAttribute(Qt::WA_Hover);
}

void AreaLabel::enterEvent(QEnterEvent *event) {
    emit enterHover();
    QLabel::enterEvent(event);
}

void AreaLabel::leaveEvent(QEvent *event) {
    emit leaveHover();
    QLabel::leaveEvent(event);
}

void AreaLabel::mousePressEvent(QMouseEvent *event) {
    emit clicked();
    QLabel::mousePressEvent(event);
}
