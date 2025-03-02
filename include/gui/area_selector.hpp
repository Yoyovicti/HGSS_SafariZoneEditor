#ifndef AREA_SELECTOR_HPP
#define AREA_SELECTOR_HPP

#include "gui/area_item.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QVBoxLayout>

#include <array>

class AreaSelector : public QWidget {
public:
    AreaSelector(QWidget *parent = nullptr);

private:
    std::array<AreaItem, SaveDataManager::N_DAY_COUNTERS> area_items_;
    QVBoxLayout layout_;

};

#endif // AREA_SELECTOR_HPP
