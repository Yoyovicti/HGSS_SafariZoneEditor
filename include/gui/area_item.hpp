#ifndef AREA_ITEM_HPP
#define AREA_ITEM_HPP

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class AreaItem : public QWidget {
    Q_OBJECT
public:
    AreaItem(QWidget *parent = nullptr);

    void setArea(uint8_t area_id);

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QLabel area_img_;
    QLabel area_name_;

    QHBoxLayout layout_;

signals:
    void enterHover();
    void leaveHover();
    void clicked();
};

#endif // AREA_ITEM_HPP
