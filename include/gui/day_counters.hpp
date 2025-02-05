#ifndef DAY_COUNTERS_HPP
#define DAY_COUNTERS_HPP

#include "manager/save_data_manager.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>

#include <array>
#include <string>

class DayCounters : public QWidget {
private:
    const std::string JSON_KEY = "zones";

    QGridLayout layout_;
    std::array<QLabel, SaveDataManager::N_DAY_COUNTERS> labels_;
    std::array<QLineEdit, SaveDataManager::N_DAY_COUNTERS> line_edits_;

public:
    DayCounters(QWidget* parent = nullptr);
    ~DayCounters();

    void fillCounters(const std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS>& counters);
    std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS> retrieveCounters();
    void updateLanguage(const uint8_t& language);

public slots:
    void highlightCounter(uint8_t& index);
    void resetHighlight(uint8_t& index);
};

#endif
