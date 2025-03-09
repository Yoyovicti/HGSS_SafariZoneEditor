#ifndef DAY_COUNTER_EDIT_HPP
#define DAY_COUNTER_EDIT_HPP

#include <QLineEdit>
#include <QIntValidator>

class DayCounterEdit : public QLineEdit {
    Q_OBJECT

public:
    DayCounterEdit(QWidget* parent = nullptr) : QLineEdit(parent) {
        setFixedWidth(50);
        setEnabled(false);
        setValidator(new QIntValidator(0, 9999, this));
        QObject::connect(this, &QLineEdit::textChanged, this, &DayCounterEdit::onTextChanged);
    }

    void setText(const QString& text) {
        QLineEdit::setText(text);
        if(!isEnabled()) setEnabled(true);
    }

signals:
    void dayCountChanged(uint8_t count);

private slots:
    void onTextChanged(const QString& text) {
        bool ok;
        int day_count = text.toUInt(&ok);
        if(!ok) day_count = 0;

        uint8_t res = std::min(day_count, 255);
        setText(QString::number(res));
        emit dayCountChanged(res);
    }
};

#endif // DAY_COUNTER_EDIT_HPP
