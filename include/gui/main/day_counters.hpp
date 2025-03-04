#ifndef DAY_COUNTERS_HPP
#define DAY_COUNTERS_HPP

#include "day_counter_edit.hpp"
#include "manager/save_data_manager.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include <array>
#include <string>

class DayCounters : public QWidget {
    Q_OBJECT
private:
    const std::string JSON_KEY = "zones";

    QGridLayout layout_;
    std::array<QLabel, SaveDataManager::N_DAY_COUNTERS> labels_;
    std::array<DayCounterEdit, SaveDataManager::N_DAY_COUNTERS> line_edits_;

public:
    DayCounters(QWidget* parent = nullptr);
    ~DayCounters();

    void fillCounters(const std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS>& counters);
    std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS> retrieveCounters();
    void updateLanguage(const uint8_t& language);
    void highlightCounter(uint8_t& index, bool enable);

signals:
    void counterChanged(uint8_t c_id, uint8_t value);

};

#endif
