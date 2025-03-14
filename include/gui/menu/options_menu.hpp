#ifndef OPTIONS_MENU_H
#define OPTIONS_MENU_H

#include "lang_menu.hpp"
#include "about_message_box.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class OptionsMenu : public QMenu {
    Q_OBJECT

public:
    inline static const std::string JSON_KEY = "options";

    OptionsMenu(QWidget* parent = nullptr);

private:
    LangMenu lang_menu_;
    AboutMessageBox about_message_box_;

signals:
    void languageChangeClicked(uint8_t locale);

private slots:
    void updateLanguage(uint8_t locale);

};

#endif // OPTIONS_MENU_H
