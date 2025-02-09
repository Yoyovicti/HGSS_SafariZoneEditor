#include "gui/object_item.hpp"

ObjectItem::ObjectItem(QWidget *parent) : QWidget(parent), layout_(this), id_(this), xyz_(this) {
    layout_.addWidget(&id_, 0, 0, 1, 1);
    layout_.addWidget(&xyz_, 1, 0, 1, 1);
}
