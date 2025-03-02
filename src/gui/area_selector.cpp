#include "gui/area_selector.hpp"

#include <iostream>

AreaSelector::AreaSelector(QWidget *parent) : QWidget(parent), layout_(this) {
    // QLabel* test_label = new QLabel("test", this);
    // layout_.addWidget(test_label);
    for(uint8_t i = 0; i < area_items_.size(); i++) {
        area_items_[i].setArea(i);
        layout_.addWidget(&area_items_[i]);
        // std::cout << int(i) << std::endl;
    }
    // adjustSize();
}
