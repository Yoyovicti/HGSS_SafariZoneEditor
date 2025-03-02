#ifndef OBJECT_VIEW_HPP
#define OBJECT_VIEW_HPP

#include "object_item.hpp"
#include "manager/save_data_manager.hpp"

class ObjectView : public QWidget {
    Q_OBJECT

public:
    ObjectView(QWidget* parent = nullptr) : QWidget(parent), layout_(this) {
        for(uint8_t i = 0; i < obj_items_.size(); i++) {
            obj_items_[i].hide();
            layout_.addWidget(&obj_items_[i]);

            QObject::connect(&obj_items_[i], &ObjectItem::enterHover, this, [this, i]() {emit enterItemHover(i);});
            QObject::connect(&obj_items_[i], &ObjectItem::leaveHover, this, [this, i]() {emit leaveItemHover(i);});
        }
    }

    int getWidth() {return obj_items_[0].sizeHint().width();}

    void setSlot(const Slot& slot) {
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

private:
    const std::string JSON_KEY = "objects";

    QVBoxLayout layout_;
    std::array<ObjectItem, 30> obj_items_;

    Slot slot_;

signals:
    void enterItemHover(uint8_t i);
    void leaveItemHover(uint8_t i);
};

#endif // OBJECT_VIEW_HPP
