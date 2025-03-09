#include "main_window.hpp"

#include "manager/locale_manager.hpp"
#include "manager/config_manager.hpp"

#include <QScrollBar>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent) : QWidget(parent), layout_(this), menu_bar_(this), file_menu_(this), options_menu_(this), area_view_(this), safari_layout_(this), day_counters_(this), edit_button_(this), area_scroll_(this), area_selector_(this), selected_area_(-1), edit_mode_(false) {
    menu_bar_.addMenu(&file_menu_);
    menu_bar_.addMenu(&options_menu_);

    area_view_.hide();
    area_scroll_.hide();

    area_scroll_.setWidget(&area_selector_);
    // Disable horizontal scrollbar
    area_scroll_.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    area_scroll_.horizontalScrollBar()->setEnabled(false);

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load main_window table" << std::endl;
        return;
    }

    ConfigManager& config_manager = ConfigManager::getInstance();
    uint8_t locale = config_manager.getLocale();

    edit_button_.setText(QString::fromStdString(table["edit_button"][locale]));
    edit_button_.setEnabled(false);

    file_label_.setText(QString::fromStdString(table["file_label"][locale]));

    layout_.setMenuBar(&menu_bar_);
    layout_.addWidget(&file_label_, 0, 0, 1, 17);
    layout_.addWidget(&safari_layout_, 1, 0, 12, 18);
    layout_.addWidget(&area_view_, 1, 0, 1, 1);
    layout_.addWidget(&day_counters_, 1, 18, 12, 2);
    layout_.addWidget(&edit_button_, 0, 17, 1, 1);
    layout_.addWidget(&area_scroll_, 1, 18, 12, 1);

    QObject::connect(&file_menu_, &FileMenu::openFileClicked, this, &MainWindow::openFileDialog);
    QObject::connect(&file_menu_, &FileMenu::saveAsFileClicked, this, &MainWindow::saveFileDialog);
    QObject::connect(&options_menu_, &OptionsMenu::languageChangeClicked, this, &MainWindow::updateLanguage);
    QObject::connect(&edit_button_, &QPushButton::released, this, &MainWindow::editButtonReleased);
    QObject::connect(&safari_layout_, &SafariLayout::areaEnterHover, this, [this](uint8_t index){highlightCounter(index, true);});
    QObject::connect(&safari_layout_, &SafariLayout::areaLeaveHover, this, [this](uint8_t index){highlightCounter(index, false);});
    QObject::connect(&safari_layout_, &SafariLayout::areaClicked, this, &MainWindow::areaClicked);
    QObject::connect(&area_view_, &AreaView::backButtonReleased, this, &MainWindow::exitAreaViewer);
    QObject::connect(&day_counters_, &DayCounters::counterChanged, this, &MainWindow::updateCounters);
    QObject::connect(&area_view_, &AreaView::counterChanged, this, &MainWindow::updateCounters);
    QObject::connect(&area_selector_, &AreaSelector::areaItemClicked, this, &MainWindow::updateSelectedArea);

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
    safari_layout_.loadData(save_data_manager_.getSlots());
    edit_button_.setEnabled(true);
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

    std::cout << "File saved successfully: " << file_name << std::endl;
}

void MainWindow::updateLanguage(uint8_t locale) {
    ConfigManager& config_manager = ConfigManager::getInstance();
    config_manager.setLocale(locale);

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load main_window table" << std::endl;
        return;
    }

    if(!save_data_manager_.isLoaded())
        file_label_.setText(QString::fromStdString(table["file_label"][locale]));
    edit_button_.setText(QString::fromStdString(table["edit_button"][locale]));
    area_selector_.updateLanguage(locale);
    day_counters_.updateLanguage(locale);
    file_menu_.updateLanguage(locale);
}

void MainWindow::areaClicked(uint8_t index) {
    if(edit_mode_) {
        selected_area_ = index;
        safari_layout_.highlightSlot(index);
        return;
    }

    // enterAreaViewer(index);
}

void MainWindow::enterAreaViewer(uint8_t index) {
    file_label_.hide();
    safari_layout_.hide();
    day_counters_.hide();
    edit_button_.hide();

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
    adjustSize();
}

void MainWindow::exitAreaViewer() {
    area_view_.hide();
    file_label_.show();
    safari_layout_.show();
    day_counters_.fillCounters(save_data_manager_.getCounters());
    day_counters_.show();
    edit_button_.show();
    adjustSize();
}

void MainWindow::updateCounters(uint8_t area_id, uint8_t value) {
    save_data_manager_.setCounter(area_id, value);
}

void MainWindow::editButtonReleased() {
    edit_mode_ = !edit_mode_;
    selected_area_ = -1;
    safari_layout_.highlightSlot(-1);

    if(edit_mode_) {
        day_counters_.hide();
        area_scroll_.show();
    } else {
        area_scroll_.hide();
        day_counters_.show();
    }
    adjustSize();
}

void MainWindow::highlightCounter(uint8_t& index, bool highlight) {
    if(edit_mode_) return;

    day_counters_.highlightCounter(index, highlight);
}

void MainWindow::updateSelectedArea(uint8_t index) {
    if(selected_area_ >= SaveDataManager::N_SLOTS)
        return;

    save_data_manager_.setAreaSlot(selected_area_, index);
    safari_layout_.updateSlot(selected_area_, index);
    emit editButtonReleased();
}
