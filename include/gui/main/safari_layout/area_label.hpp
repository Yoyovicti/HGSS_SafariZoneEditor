#ifndef AREA_LABEL_HPP
#define AREA_LABEL_HPP

#include "gui/qselectitem.hpp"

#include <QLabel>
#include <QVBoxLayout>
#include <QEvent>
#include <QEnterEvent>

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
    QLabel img_label_;
    QVBoxLayout layout_;
};

#endif // HOVER_LABEL_HPP
