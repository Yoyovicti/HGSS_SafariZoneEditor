#include "gui/file_menu.hpp"

FileMenu::FileMenu(QWidget* parent) : QMenu(parent) {
    ConfigManager& config_manager = ConfigManager::getInstance();
    uint8_t locale = config_manager.getLocale();

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json file_table;
    if(!locale_manager.getTable(&file_table, JSON_KEY)) {
        std::cerr << "Unable to load file table" << std::endl;
        return;
    }

    json menubar_table;
    if(!locale_manager.getTable(&menubar_table, "menubar")) {
        std::cerr << "Unable to load menubar table" << std::endl;
        return;
    }

    addAction(QString::fromStdString(file_table["open"][locale]), [&](){emit openFileClicked();});
    addAction(QString::fromStdString(file_table["save_as"][locale]), [&](){emit saveAsFileClicked();});
    this->setTitle(QString::fromStdString(menubar_table[JSON_KEY][locale]));
}

FileMenu::~FileMenu() {}

void FileMenu::updateLanguage(uint8_t& locale) {
    LocaleManager& locale_manager = LocaleManager::getInstance();
    json file_table;
    if(!locale_manager.getTable(&file_table, JSON_KEY)) {
        std::cerr << "Unable to load file data" << std::endl;
        return;
    }

    json menubar_table;
    if(!locale_manager.getTable(&menubar_table, "menubar")) {
        std::cerr << "Unable to load menubar table" << std::endl;
        return;
    }

    QList<QAction*> action_list = actions();
    action_list[0]->setText(QString::fromStdString(file_table["open"][locale]));
    action_list[1]->setText(QString::fromStdString(file_table["save_as"][locale]));
    this->setTitle(QString::fromStdString(menubar_table[JSON_KEY][locale]));
}
