#include "day_counters.hpp"

#include "manager/locale_manager.hpp"

DayCounters::DayCounters(QWidget* parent) : QWidget(parent), layout_(this), line_edits_() {

    for(uint8_t i = 0; i < line_edits_.size(); i++) {

        DayCounterEdit* line_edit = &line_edits_[i];
        // line_edit->setFixedWidth(50);
        // line_edit->setDisabled(true);

        QLabel* label = &labels_[i];

        layout_.addWidget(label, i, 0, 1, 1);
        layout_.addWidget(line_edit, i, 1, 1, 1);

        QObject::connect(line_edit, &DayCounterEdit::dayCountChanged, this, [&](uint8_t count) {
            emit counterChanged(i, count);
        });
    }

    this->setLayout(&layout_);
}

DayCounters::~DayCounters() {}

void DayCounters::fillCounters(const std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS>& counters) {
    for(uint8_t i = 0; i < counters.size(); i++) {
        line_edits_[i].setText(QString::number(counters[i]));
        line_edits_[i].setEnabled(true);
    }
}

std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS> DayCounters::retrieveCounters() {
    std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS> counters;
    for(uint32_t i = 0; i < counters.size(); i++) {
        counters[i] = std::min(255U, line_edits_[i].text().toUInt());
    }
    return counters;
}

void DayCounters::updateLanguage(const uint8_t& language) {
    LocaleManager& locale_manager = LocaleManager::getInstance();
    json table;
    if(!locale_manager.getTable(&table, JSON_KEY)) {
        std::cerr << "Unable to load zones table" << std::endl;
        return;
    }

    for(uint8_t i = 0; i < SaveDataManager::N_DAY_COUNTERS; i++){
        labels_[i].setText(QString::fromStdString(table[i][language]));
    }
}

void DayCounters::highlightCounter(uint8_t& index) {
    line_edits_[index].setStyleSheet("QLineEdit { background-color: rgba(100, 100, 255, 80); }");
}

void DayCounters::resetHighlight(uint8_t& index) {
    line_edits_[index].setStyleSheet("");
}
