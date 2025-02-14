#include "gui/main_window.hpp"

#include "manager/locale_manager.hpp"
#include "manager/config_manager.hpp"

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), layout_(this), menu_bar_(this), file_menu_(this), options_menu_(this), area_view_(this), /*area_view_old_(this), */layout_view_(this), day_counters_(this) {
    menu_bar_.addMenu(&file_menu_);
    menu_bar_.addMenu(&options_menu_);

    area_view_.hide();
    // area_view_old_.hide();

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
    layout_.addWidget(&file_label_, 0, 0, 1, 20);
    layout_.addWidget(&layout_view_, 1, 0, 12, 18);
    layout_.addWidget(&area_view_, 1, 0, 1, 1);
    // layout_.addWidget(&area_view_old_, 1, 0, 1, 1);
    layout_.addWidget(&day_counters_, 1, 18, 12, 2);
    this->setLayout(&layout_);

    QObject::connect(&file_menu_, &FileMenu::openFileClicked, this, &MainWindow::openFileDialog);
    QObject::connect(&file_menu_, &FileMenu::saveAsFileClicked, this, &MainWindow::saveFileDialog);
    QObject::connect(&options_menu_, &OptionsMenu::languageChangeClicked, this, &MainWindow::updateLanguage);
    QObject::connect(&layout_view_, &SafariLayoutView::areaHovered, &day_counters_, &DayCounters::highlightCounter);
    QObject::connect(&layout_view_, &SafariLayoutView::areaLeaveHover, &day_counters_, &DayCounters::resetHighlight);
    QObject::connect(&layout_view_, &SafariLayoutView::areaClicked, this, &MainWindow::enterAreaViewer);
    QObject::connect(&area_view_, &AreaView::backButtonReleased, this, &MainWindow::exitAreaViewer);
    QObject::connect(&day_counters_, &DayCounters::counterChanged, this, &MainWindow::updateCounters);
    QObject::connect(&area_view_, &AreaView::counterChanged, this, &MainWindow::updateCounters);

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

    save_data_manager_.readData(selected_file);

    day_counters_.fillCounters(save_data_manager_.getCounters());
    layout_view_.loadData(save_data_manager_.getSlots());
}

void MainWindow::saveFileDialog() {
    if(!save_data_manager_.isLoaded()) {
        std::cerr << "No data to save" << std::endl;
        return;
    }

    ConfigManager& config_manager = ConfigManager::getInstance();
    std::string default_path = config_manager.getDefaultPath();

    std::filesystem::path file_name = QFileDialog::getSaveFileName(this, "Save file as", QString::fromStdString(default_path), "HGSS save files (*.sav)").toStdString();
    if(!save_data_manager_.writeData(file_name)) {
        std::cerr << "Unable to save data at: " << file_name << std::endl;
        return;
    }
    config_manager.setDefaultPath(file_name.parent_path().string());

    // std::cout << "File saved successfully: " << file_name << std::endl;
}

void MainWindow::updateLanguage(uint8_t locale) {
    ConfigManager& config_manager = ConfigManager::getInstance();
    config_manager.setLocale(locale);

    if(!save_data_manager_.isLoaded()) {
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
}

void MainWindow::enterAreaViewer(size_t index) {
    file_label_.hide();
    layout_view_.hide();
    day_counters_.hide();

    const Slot& slot = save_data_manager_.getSlots()[index];

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json zone_table;
    if(!locale_manager.getTable(&zone_table, "zones")) {
        std::cerr << "Unable to load zones table" << std::endl;
        return;
    }
    std::string area = zone_table[slot.area_type_][0];

    std::filesystem::path assets_path = "assets";
    std::filesystem::path map_path = assets_path / "maps" / area;

    std::cout << "Loading area: " << map_path << std::endl;

    area_view_.setDayCount(save_data_manager_.getCounters()[slot.area_type_]);
    area_view_.setSlot(slot);
    area_view_.setModelDir(map_path);
    area_view_.show();
}

void MainWindow::exitAreaViewer() {
    area_view_.hide();
    file_label_.show();
    layout_view_.show();
    day_counters_.fillCounters(save_data_manager_.getCounters());
    day_counters_.show();
}

void MainWindow::updateCounters(uint8_t area_id, uint8_t value) {
    std::cout << int(area_id) << " " << int(value) << std::endl;
    save_data_manager_.setCounter(area_id, value);
}
