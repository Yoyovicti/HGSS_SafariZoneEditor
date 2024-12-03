#include "gui/about_message_box.hpp"

AboutMessageBox::AboutMessageBox(QWidget* parent) : QMessageBox(parent) {
    ConfigManager& config_manager = ConfigManager::getInstance();
    uint8_t locale = config_manager.getLocale();

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load about table" << std::endl;
        return;
    }

    json options_table;
    if(!locale_manager.getTable(&options_table, "options")) {
        std::cerr << "Unable to load options table" << std::endl;
        return;
    }
    
    this->setText(QString::fromStdString(table["message"][locale]));
    this->setWindowTitle(QString::fromStdString(options_table[JSON_KEY][locale]));
}

void AboutMessageBox::updateLanguage(uint8_t& locale) {
    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load about table" << std::endl;
        return;
    }

    json options_table;
    if(!locale_manager.getTable(&options_table, "options")) {
        std::cerr << "Unable to load options table" << std::endl;
        return;
    }

    this->setText(QString::fromStdString(table["message"][locale]));
    this->setWindowTitle(QString::fromStdString(options_table[JSON_KEY][locale]));
}
