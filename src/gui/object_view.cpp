#include "gui/object_view.hpp"

ObjectView::ObjectView(QWidget* parent) : QWidget(parent), layout_(this) {
    for(uint8_t i = 0; i < obj_items_.size(); i++) {
        obj_items_[i].hide();
        layout_.addWidget(&obj_items_[i]);

        QObject::connect(&obj_items_[i], &ObjectItem::enterHover, this, [&, i]() {emit enterItemHover(i);});
        QObject::connect(&obj_items_[i], &ObjectItem::leaveHover, this, [&, i]() {emit leaveItemHover(i);});
    }
}

void ObjectView::setSlot(const Slot& slot) {
    slot_ = slot;

    for(uint8_t i = 0; i < obj_items_.size(); i++) {
        Object& obj = slot_.objects_[i];
        if(i >= slot.object_count_) {
            obj_items_[i].hide();
            continue;
        }

        std::stringstream ss;
        ss << int(obj.x_) << " " << int(obj.y_) << " " << int(obj.z_);
        obj_items_[i].setObject(obj);
        obj_items_[i].show();
    }

    adjustSize();
}
