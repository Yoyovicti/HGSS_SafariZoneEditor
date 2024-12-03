#include "gui/options_menu.hpp"

OptionsMenu::OptionsMenu(QWidget* parent) : QMenu(parent), lang_menu_(this), about_message_box_(this) {
    ConfigManager& config_manager = ConfigManager::getInstance();
    uint8_t locale = config_manager.getLocale();

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load options table" << std::endl;
        return;
    }

    json menubar_table;
    if(!locale_manager.getTable(&menubar_table, "menubar")) {
        std::cerr << "Unable to load menubar table" << std::endl;
        return;
    }

    addMenu(&lang_menu_);
    addAction(QString::fromStdString(table["about"][locale]), [this](){about_message_box_.show();});
    this->setTitle(QString::fromStdString(menubar_table[JSON_KEY][locale]));

    QObject::connect(&lang_menu_, &LangMenu::languageChangeClicked, this, &OptionsMenu::updateLanguage);
}

void OptionsMenu::updateLanguage(uint8_t locale) {
    lang_menu_.updateLanguage(locale);
    about_message_box_.updateLanguage(locale);

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json options_table;
    if(!locale_manager.getTable(&options_table, JSON_KEY)) {
        std::cerr << "Unable to load options table" << std::endl;
        return;
    }

    json menubar_table;
    if(!locale_manager.getTable(&menubar_table, "menubar")) {
        std::cerr << "Unable to load menubar table" << std::endl;
        return;
    }

    QList<QAction*> action_list = actions();
    action_list[1]->setText(QString::fromStdString(options_table["about"][locale]));
    this->setTitle(QString::fromStdString(menubar_table[JSON_KEY][locale]));

    emit languageChangeClicked(locale);
}
