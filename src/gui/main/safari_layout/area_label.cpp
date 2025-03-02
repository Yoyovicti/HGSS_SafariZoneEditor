#include "safari_layout/area_label.hpp"

AreaLabel::AreaLabel(QWidget* parent) : QLabel(parent), selected_(false) {
    setObjectName("area_label");
    setAttribute(Qt::WA_StyledBackground, true);

    setAlignment(Qt::AlignCenter);
}

void AreaLabel::setSelected(bool selected) {
    selected_ = selected;

    if(selected_) setStyleSheet("#area_label { background-color: rgba(100, 100, 255, 60); }");
    else setStyleSheet("");
}

void AreaLabel::enterEvent(QEnterEvent *event) {
    if(!isEnabled() || selected_) return;

    setStyleSheet("#area_label { background-color: rgba(100, 100, 255, 60); }");
    emit enterHover();
    QLabel::enterEvent(event);
}

void AreaLabel::leaveEvent(QEvent *event) {
    if(!isEnabled() || selected_) return;

    setStyleSheet("");
    emit leaveHover();
    QLabel::leaveEvent(event);
}

void AreaLabel::mouseReleaseEvent(QMouseEvent *event) {
    emit clicked();
    QLabel::mouseReleaseEvent(event);
}
