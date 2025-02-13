#ifndef OBJECT_VIEW_HPP
#define OBJECT_VIEW_HPP

#include "gui/object_item.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class ObjectView : public QWidget {
public:
    ObjectView(QWidget* parent = nullptr);

    int getWidth() {return obj_items_[0].sizeHint().width();}

    void setSlot(const Slot& slot);

private:
    const std::string JSON_KEY = "objects";

    QVBoxLayout layout_;
    std::array<ObjectItem, 30> obj_items_;

    Slot slot_;
};

#endif // OBJECT_VIEW_HPP
