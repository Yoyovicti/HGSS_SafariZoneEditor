#ifndef HOVER_LABEL_HPP
#define HOVER_LABEL_HPP

#include <QLabel>
#include <QEvent>
#include <QEnterEvent>

class HoverLabel : public QLabel {
    Q_OBJECT

public:
    HoverLabel(QWidget* parent = nullptr);

private:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;

signals:
    void enterHover();
    void leaveHover();
};

#endif // HOVER_LABEL_HPP
