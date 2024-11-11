#include "main_window.hpp"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), layout_(this), menu_bar_(this), file_menu_(this), lang_menu_(this), day_counters_(this) {
    menu_bar_.addMenu(&file_menu_);
    menu_bar_.addMenu(&lang_menu_);

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load main_window table" << std::endl;
        return;
    }

    ConfigManager& config_manager = ConfigManager::getInstance();
    uint8_t locale = config_manager.getLocale();

    file_label_.setText(QString::fromStdString(table["file_label"][locale]));

    layout_.setMenuBar(&menu_bar_);
    layout_.addWidget(&file_label_);
    layout_.addWidget(&day_counters_);
    this->setLayout(&layout_);

    QObject::connect(&file_menu_, &FileMenu::openFileClicked, this, &MainWindow::openFileDialog);
    QObject::connect(&file_menu_, &FileMenu::saveAsFileClicked, this, &MainWindow::saveFileDialog);
    QObject::connect(&lang_menu_, &LangMenu::languageChangeClicked, this, &MainWindow::updateLanguage);

    day_counters_.updateLanguage(locale);
}

MainWindow::~MainWindow() {
    ConfigManager& config_manager = ConfigManager::getInstance();
    config_manager.saveConfig();
}

void MainWindow::openFileDialog(){
    ConfigManager& config_manager = ConfigManager::getInstance();
    std::string default_path = config_manager.getDefaultPath();

    std::filesystem::path selected_file = QFileDialog::getOpenFileName(this, "Open file", QString::fromStdString(default_path), "HGSS save files (*.sav)").toStdString();
    if(selected_file.empty()){
        std::cout << "Operation canceled by user" << std::endl;
        return;
    }
    config_manager.setDefaultPath(selected_file.parent_path().string());

    // Update file label
    std::string label_text = selected_file.string();
    if(label_text.length() > 100)
        label_text = label_text.substr(0, 97) + "...";
    file_label_.setText(QString::fromStdString(label_text));

    SaveFileManager::readData(contents_, selected_file);
    std::cout << "File selected: " << selected_file << " (" << contents_.size() << " bytes)" << std::endl;

    day_counters_.fillCounters(contents_);
}

void MainWindow::saveFileDialog() {
    if(contents_.empty()) {
        std::cerr << "No data to save" << std::endl;
        return;
    }

    ConfigManager& config_manager = ConfigManager::getInstance();
    std::string default_path = config_manager.getDefaultPath();

    day_counters_.retrieveCounters(contents_);

    std::filesystem::path file_name = QFileDialog::getSaveFileName(this, "Save file as", QString::fromStdString(default_path), "HGSS save files (*.sav)").toStdString();
    if(!SaveFileManager::saveData(contents_, file_name)) {
        std::cerr << "Unable to save data at: " << file_name << std::endl;
        return;
    }
    config_manager.setDefaultPath(file_name.parent_path().string());

    std::cout << "File saved successfully: " << file_name << std::endl;
}

void MainWindow::updateLanguage(uint8_t locale) {
    ConfigManager& config_manager = ConfigManager::getInstance();
    config_manager.setLocale(locale);

    if(contents_.empty()) {
        LocaleManager& locale_manager = LocaleManager::getInstance();
        json table;
        if(!locale_manager.getTable(&table, JSON_KEY)) {
            std::cerr << "Unable to load main_window table" << std::endl;
            return;
        }

        file_label_.setText(QString::fromStdString(table["file_label"][locale]));
    }

    day_counters_.updateLanguage(locale);
    file_menu_.updateLanguage(locale);
    lang_menu_.updateLanguage(locale);
}
