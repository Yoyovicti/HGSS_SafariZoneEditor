#include "gui/object_view.hpp"

// #include "manager/locale_manager.hpp"

ObjectView::ObjectView(QWidget* parent) : QWidget(parent), layout_(this) {
    for(uint8_t i = 0; i < obj_items_.size(); i++) {
        obj_items_[i].hide();
        layout_.addWidget(&obj_items_[i]);

        QObject::connect(&obj_items_[i], &ObjectItem::enterHover, this, [&, i]() {emit enterItemHover(i);});
        QObject::connect(&obj_items_[i], &ObjectItem::leaveHover, this, [&, i]() {emit leaveItemHover(i);});
    }

    // adjustSize();
    // setLayout(&layout_);
}

void ObjectView::setSlot(const Slot& slot) {
    slot_ = slot;

    // LocaleManager& locale_manager = LocaleManager::getInstance();
    // json object_table;
    // if(!locale_manager.getTable(&object_table, JSON_KEY)) {
    //     std::cerr << "Unable to load blocks table" << std::endl;
    //     return;
    // }

    for(uint8_t i = 0; i < obj_items_.size(); i++) {
        Object& obj = slot_.objects_[i];
        if(i >= slot.object_count_) {
            obj_items_[i].hide();
            continue;
        }

        std::stringstream ss;
        ss << int(obj.x_) << " " << int(obj.y_) << " " << int(obj.z_);
        obj_items_[i].setObject(obj);
        // obj_labels_[i].setId(QString::fromStdString(object_table[obj.id_][0]));
        // obj_labels_[i].setXYZ(QString::fromStdString(ss.str()));
        obj_items_[i].show();
    }

    adjustSize();
    // setFixedWidth(obj_items_[0].width());


}
