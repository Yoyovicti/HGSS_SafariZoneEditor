#include "area_item.hpp"

#include "manager/locale_manager.hpp"

#include <iostream>

AreaItem::AreaItem(QWidget *parent) : QWidget(parent), /*area_img_(this), */area_name_(this), layout_(this) {
    setObjectName("area_item");
    setAttribute(Qt::WA_StyledBackground, true);

    layout_.addWidget(&area_img_);
    layout_.addWidget(&area_name_);
}

void AreaItem::setArea(uint8_t area_id) {
    LocaleManager& locale_manager = LocaleManager::getInstance();
    json zone_table;
    if(!locale_manager.getTable(&zone_table, "zones")) {
        std::cerr << "Unable to load zones table" << std::endl;
        return;
    }

    std::string area_name = zone_table[area_id][0];
    area_name_.setText(area_name.c_str());

    std::filesystem::path area_dir("assets/icons");
    std::filesystem::path area_path(area_dir / (area_name + ".png"));
    QImage area_img(area_path.string().c_str());
    area_img_.setPixmap(QPixmap::fromImage(area_img));
}

void AreaItem::enterEvent(QEnterEvent *event) {
    setStyleSheet("#area_item { background-color: rgba(100, 100, 255, 60); }");
    emit enterHover();

}

void AreaItem::leaveEvent(QEvent *event) {
    setStyleSheet("");
    emit leaveHover();
}

void AreaItem::mouseReleaseEvent(QMouseEvent *event) {
    emit clicked();
    QWidget::mouseReleaseEvent(event);
}
