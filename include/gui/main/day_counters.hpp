#ifndef DAY_COUNTERS_HPP
#define DAY_COUNTERS_HPP

#include "day_counter_edit.hpp"
#include "manager/save_data_manager.hpp"

#include <QGridLayout>
#include <QLabel>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

class DayCounters : public QWidget {
    Q_OBJECT

public:
    inline static const std::string JSON_KEY = "zones";

    DayCounters(QWidget* parent = nullptr);
    ~DayCounters();

    void fillCounters(const std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS>& counters);
    std::array<uint8_t, SaveDataManager::N_DAY_COUNTERS> retrieveCounters();
    void updateLanguage(const uint8_t& language);
    void highlightCounter(uint8_t& index, bool enable);

private:
    QGridLayout layout_;
    std::array<QLabel, SaveDataManager::N_DAY_COUNTERS> labels_;
    std::array<DayCounterEdit, SaveDataManager::N_DAY_COUNTERS> line_edits_;

signals:
    void counterChanged(uint8_t c_id, uint8_t value);
};

#endif
