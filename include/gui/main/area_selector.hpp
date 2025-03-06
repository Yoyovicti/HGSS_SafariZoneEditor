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
    AreaSelector(QWidget *parent = nullptr) : QWidget(parent), layout_(this) {
        for(uint8_t i = 0; i < area_items_.size(); i++) {
            area_items_[i].setArea(i);
            layout_.addWidget(&area_items_[i]);
            QObject::connect(&area_items_[i], &AreaItem::clicked, this, [this, i](){emit areaItemClicked(i);});
        }
    }

    void updateLanguage(uint8_t locale) {
        for(uint8_t i = 0; i < area_items_.size(); i++) {
            area_items_[i].updateLanguage(i, locale);
        }
    }

private:
    std::array<AreaItem, SaveDataManager::N_DAY_COUNTERS> area_items_;
    QVBoxLayout layout_;

signals:
    void areaItemClicked(size_t index);
};

#endif // AREA_SELECTOR_HPP
