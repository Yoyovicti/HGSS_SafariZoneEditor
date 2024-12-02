#include "day_counters.hpp"

DayCounters::DayCounters(QWidget* parent) : QWidget(parent), layout_(this), line_edits_() {

    for(size_t i = 0; i < line_edits_.size(); i++) {

        QLineEdit* line_edit = &line_edits_[i];
        line_edit->setFixedWidth(50);
        line_edit->setDisabled(true);

        QLabel* label = &labels_[i];

        layout_.addWidget(label, i, 0, 1, 1);
        layout_.addWidget(line_edit, i, 1, 1, 1);
    }

    this->setLayout(&layout_);
}

DayCounters::~DayCounters() {}

void DayCounters::fillCounters(const std::vector<unsigned char>& data) {
    size_t file_size = data.size();

    if(file_size <= SaveFileManager::SAFARI_OFFSET + SaveFileManager::COUNTER_SIZE) {
        std::cerr << "Invalid file size: " << file_size << std::endl;
        return;
    }

    for(uint8_t i = 0; i < SaveFileManager::COUNTER_SIZE; i++) {
        line_edits_[i].setText(QString::number(data[i + SaveFileManager::SAFARI_OFFSET]));
        line_edits_[i].setEnabled(true);
    }
}

void DayCounters::retrieveCounters(std::vector<unsigned char>& data) {

    size_t file_size = data.size();

    if(file_size <= SaveFileManager::SAFARI_OFFSET + SaveFileManager::COUNTER_SIZE) {
        std::cerr << "Invalid file size: " << file_size << std::endl;
        return;
    }

    for(uint32_t i = 0; i < SaveFileManager::COUNTER_SIZE; i++) {
        data[i + SaveFileManager::SAFARI_OFFSET] = std::min(255U, line_edits_[i].text().toUInt());
    }
}

void DayCounters::updateLanguage(const uint8_t& language) {
    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load zones table" << std::endl;
        return;
    }

    for(uint8_t i = 0; i < SaveFileManager::COUNTER_SIZE; i++){
        labels_[i].setText(QString::fromStdString(table[i][language]));
    }
}

void DayCounters::highlightCounter(uint8_t& index) {
    line_edits_[index].setStyleSheet("QLineEdit { background-color: rgba(100, 100, 255, 80); }");
}

void DayCounters::resetHighlight(uint8_t& index) {
    line_edits_[index].setStyleSheet("");
}
