#ifndef ABOUT_MESSAGE_BOX_HPP
#define ABOUT_MESSAGE_BOX_HPP

#include "manager/config_manager.hpp"
#include "manager/locale_manager.hpp"

#include <QMessageBox>

#include <string>

class AboutMessageBox : public QMessageBox {
private:
    const std::string JSON_KEY = "about";

public:
    AboutMessageBox(QWidget* parent = nullptr);

    void updateLanguage(uint8_t& locale);
};

#endif
