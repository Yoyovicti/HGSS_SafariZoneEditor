#ifndef LANG_MENU_HPP
#define LANG_MENU_HPP

#include "locale_manager.hpp"
#include "config_manager.hpp"

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include <QMenu>
#include <QActionGroup>

#include <iostream>

class LangMenu : public QMenu {
    Q_OBJECT

private:
    QActionGroup action_group_;

    const std::string JSON_KEY = "locale";

public:
    LangMenu(QWidget *parent = nullptr);
    ~LangMenu();

    void updateLanguage(uint8_t& locale);

signals:
    void languageChangeClicked(uint8_t locale); 
};

#endif
