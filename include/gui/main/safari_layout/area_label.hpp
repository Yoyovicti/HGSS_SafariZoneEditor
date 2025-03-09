#ifndef AREA_LABEL_HPP
#define AREA_LABEL_HPP

#include "gui/qselectitem.hpp"

#include <QVBoxLayout>
#include <QLabel>

class AreaLabel : public QSelectItem {
public:
    AreaLabel(QWidget* parent = nullptr) : QSelectItem("area_label", parent), img_label_(this), layout_(this) {
        setFixedSize(150, 150);
        img_label_.setAlignment(Qt::AlignCenter);
        layout_.addWidget(&img_label_);
    }

    void setPixmap(const QPixmap& pixmap) {
        img_label_.setPixmap(pixmap);
    }

private:
    QVBoxLayout layout_;
    QLabel img_label_;
};

#endif // HOVER_LABEL_HPP
