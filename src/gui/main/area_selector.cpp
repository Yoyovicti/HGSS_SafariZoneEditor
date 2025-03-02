#include "main/area_selector.hpp"

AreaSelector::AreaSelector(QWidget *parent) : QWidget(parent), layout_(this) {
    for(uint8_t i = 0; i < area_items_.size(); i++) {
        area_items_[i].setArea(i);
        layout_.addWidget(&area_items_[i]);
        QObject::connect(&area_items_[i], &AreaItem::clicked, this, [this, i](){emit areaItemClicked(i);});
    }
}
