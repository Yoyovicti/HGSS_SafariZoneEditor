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

    void updateLanguage(const uint8_t& language);
private:
    const Object* object_;

    QGridLayout layout_;

    QLabel icon_label_;
    QLabel object_label_;

    QLabel x_label_;
    QLabel cx_label_;
    QLabel y_label_;
    QLabel cy_label_;
};

#endif // OBJECT_ITEM_HPP
