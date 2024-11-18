#include "lang_menu.hpp"

LangMenu::LangMenu(QWidget *parent) : QMenu(parent), action_group_(this) {
    ConfigManager& config_manager = ConfigManager::getInstance();
    uint8_t locale = config_manager.getLocale();

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load locale table" << std::endl;
        return;
    }

    json options_table;
    if(!locale_manager.getTable(&options_table, "options")) {
        std::cerr << "Unable to load options table" << std::endl;
        return;
    }

    for(uint8_t i = 0; i < table.size(); i++) {
        QAction* action = addAction(QString::fromStdString(table[i][locale]), [this, i](){emit languageChangeClicked(i);});
        action->setCheckable(true);
        if(i == locale) action->setChecked(true);
        action_group_.addAction(action);
    }
    this->setTitle(QString::fromStdString(options_table[JSON_KEY][locale]));
}

LangMenu::~LangMenu() {}

void LangMenu::updateLanguage(uint8_t& locale) {
    LocaleManager& locale_manager = LocaleManager::getInstance();
    json language_table;
    if(!locale_manager.getTable(&language_table, JSON_KEY)) {
        std::cerr << "Unable to load locale table" << std::endl;
        return;
    }

    json options_table;
    if(!locale_manager.getTable(&options_table, "options")) {
        std::cerr << "Unable to load options table" << std::endl;
        return;
    }
    
    QList<QAction*> action_list = actions();
    for(uint8_t i = 0; i < language_table.size(); i++) {
        QAction* action = action_list[i];
        action->setText(QString::fromStdString(language_table[i][locale]));
    }
    this->setTitle(QString::fromStdString(options_table[JSON_KEY][locale]));
}