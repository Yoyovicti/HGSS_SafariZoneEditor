#ifndef OBJECT_ITEM_HPP
#define OBJECT_ITEM_HPP

#include "gui/qselectitem.hpp"
#include "manager/save_data_manager.hpp"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

class ObjectItem : public QSelectItem {
public:
    ObjectItem(QWidget *parent = nullptr);

    void setObject(const Object& object);

private:
    QGridLayout layout_;

    QLabel icon_label_;
    QLabel object_label_;

    QLabel x_label_;
    QLabel cx_label_;
    QLabel y_label_;
    QLabel cy_label_;

    QPushButton move_button_;
    QPushButton delete_button_;
};

#endif // OBJECT_ITEM_HPP
