#ifndef OBJECT_ITEM_HPP
#define OBJECT_ITEM_HPP

#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
// #include <QGraphicsColorizeEffect>
#include <QIcon>

class ObjectItem : public QWidget {
public:
    ObjectItem(QWidget *parent = nullptr);

    void setObject(const Object& object);
    // void setXYZ(const QString& text){xyz_.setText(text);}

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

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

    // QGraphicsColorizeEffect color_effect_;

    QIcon loadAndRecolorIcon(const QString& img_path, const QColor& color);
};

#endif // OBJECT_ITEM_HPP
