#ifndef ABOUT_MESSAGE_BOX_HPP
#define ABOUT_MESSAGE_BOX_HPP

#include <QMessageBox>

class AboutMessageBox : public QMessageBox {
public:
    inline static const std::string JSON_KEY = "about";

    AboutMessageBox(QWidget* parent = nullptr);

    void updateLanguage(uint8_t& locale);
};

#endif
