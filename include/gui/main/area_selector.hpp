#ifndef AREA_SELECTOR_HPP
#define AREA_SELECTOR_HPP

#include "area_item.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QVBoxLayout>

#include <array>

class AreaSelector : public QWidget {
    Q_OBJECT
public:
    AreaSelector(QWidget *parent = nullptr);

private:
    std::array<AreaItem, SaveDataManager::N_DAY_COUNTERS> area_items_;
    QVBoxLayout layout_;

signals:
    void areaItemClicked(size_t index);
};

#endif // AREA_SELECTOR_HPP
