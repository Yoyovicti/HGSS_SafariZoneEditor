#ifndef QSELECTITEM_HPP
#define QSELECTITEM_HPP

#include <QWidget>
#include <QStyle>

#include <iostream>

class QSelectItem : public QWidget {
    Q_OBJECT
public:
    QSelectItem(const QString& obj_name, QWidget *parent = nullptr) : QWidget(parent), obj_name_(obj_name), selected_(false) {
        setObjectName(obj_name);
        setAttribute(Qt::WA_StyledBackground, true);
    }

    void setSelected(bool selected) {
        selected_ = selected;

        QString stylesheet = "";
        if (selected_)
            stylesheet = "#" + obj_name_ + " { background-color: rgba(100, 100, 255, 60); }";
        setStyleSheet(stylesheet);
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if(!isEnabled()) return;
        emit clicked();
        QWidget::mouseReleaseEvent(event);
    }

    void enterEvent(QEnterEvent *event) override {
        if(!isEnabled() || selected_) return;
        setStyleSheet("#" + obj_name_ + " { background-color: rgba(100, 100, 255, 60); }");
        emit enterHover();
        QWidget::enterEvent(event);
    }

    void leaveEvent(QEvent *event) override {
        if(!isEnabled() || selected_) return;
        setStyleSheet("");
        emit leaveHover();
        QWidget::leaveEvent(event);
    }

private:
    QString obj_name_;
    bool selected_;

signals:
    void clicked();
    void enterHover();
    void leaveHover();
};

#endif // QSELECTITEM_HPP
