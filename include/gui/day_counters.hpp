#ifndef DAY_COUNTERS_HPP
#define DAY_COUNTERS_HPP

#include "manager/locale_manager.hpp"
#include "manager/save_file_manager.hpp"

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <QWidget>
#include <QGridLayout>
#include <QLineEdit>
#include <QLabel>

#include <iostream>
#include <array>
#include <string>

class DayCounters : public QWidget {
private:
    const std::string JSON_KEY = "zones";

    QGridLayout layout_;
    std::array<QLabel, SaveFileManager::COUNTER_SIZE> labels_;
    std::array<QLineEdit, SaveFileManager::COUNTER_SIZE> line_edits_;

public:
    DayCounters(QWidget* parent = nullptr);
    ~DayCounters();

    void fillCounters(const std::vector<unsigned char>& data);
    void retrieveCounters(std::vector<unsigned char>& data);
    void updateLanguage(const uint8_t& language);

public slots:
    void highlightCounter(uint8_t& index);
    void resetHighlight(uint8_t& index);
};

#endif
