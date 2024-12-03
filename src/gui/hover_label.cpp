#include "gui/hover_label.hpp"

HoverLabel::HoverLabel(QWidget* parent) : QLabel(parent) {
    setAttribute(Qt::WA_Hover);
}

void HoverLabel::enterEvent(QEnterEvent *event) {
    emit enterHover();
    QLabel::enterEvent(event);
}

void HoverLabel::leaveEvent(QEvent *event) {
    emit leaveHover();
    QLabel::leaveEvent(event);
}
