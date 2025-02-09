#ifndef OBJECT_VIEW_HPP
#define OBJECT_VIEW_HPP

#include "gui/object_item.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

class ObjectView : public QWidget {
public:
    ObjectView(QWidget* parent = nullptr);

    void setSlot(const Slot& slot);

private:
    const std::string JSON_KEY = "objects";

    QGridLayout layout_;
    std::array<ObjectItem, 30> obj_labels_;

    Slot slot_;
};

#endif // OBJECT_VIEW_HPP
