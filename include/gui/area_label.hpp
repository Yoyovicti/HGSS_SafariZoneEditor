#ifndef AREA_LABEL_HPP
#define AREA_LABEL_HPP

#include <QLabel>
#include <QEvent>
#include <QEnterEvent>

class AreaLabel : public QLabel {
    Q_OBJECT

public:
    AreaLabel(QWidget* parent = nullptr);

    void setHighlight(bool enabled = true);
    void setSelected(bool selected);

private:
    bool selected_;

    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void enterHover();
    void leaveHover();
    void clicked();
};

#endif // HOVER_LABEL_HPP
