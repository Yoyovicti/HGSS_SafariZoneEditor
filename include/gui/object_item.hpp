#ifndef OBJECT_ITEM_HPP
#define OBJECT_ITEM_HPP

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

class ObjectItem : public QWidget {
public:
    ObjectItem(QWidget *parent = nullptr);

    void setId(const QString& text){id_.setText(text);}
    void setXYZ(const QString& text){xyz_.setText(text);}

private:
    QGridLayout layout_;

    QLabel id_;
    QLabel xyz_;
};

#endif // OBJECT_ITEM_HPP
