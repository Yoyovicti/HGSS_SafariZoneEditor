#ifndef AREA_ITEM_HPP
#define AREA_ITEM_HPP

#include "gui/qselectitem.hpp"
#include "manager/locale_manager.hpp"
#include "manager/config_manager.hpp"

#include <QHBoxLayout>
#include <QLabel>

class AreaItem : public QSelectItem {
public:
    AreaItem(QWidget *parent = nullptr) : QSelectItem("area_item", parent), area_name_(this), layout_(this) {
        layout_.addWidget(&area_img_);
        layout_.addWidget(&area_name_);
    }

    void setArea(uint8_t area_id) {
        LocaleManager& locale_manager = LocaleManager::getInstance();
        json zone_table;
        if(!locale_manager.getTable(&zone_table, "zones")) {
            std::cerr << "Unable to load zones table" << std::endl;
            return;
        }

        ConfigManager& config_manager = ConfigManager::getInstance();
        uint8_t locale = config_manager.getLocale();
        area_name_.setText(QString::fromStdString(zone_table[area_id][locale]));

        std::string name_en = zone_table[area_id][0];
        std::filesystem::path area_dir("assets/icons");
        std::filesystem::path area_path(area_dir / (name_en + ".png"));
        QImage area_img(QString::fromStdString(area_path.string()));
        area_img_.setPixmap(QPixmap::fromImage(area_img));
    }

    void updateLanguage(uint8_t area_id, uint8_t locale) {
        LocaleManager& locale_manager = LocaleManager::getInstance();
        json zone_table;
        if(!locale_manager.getTable(&zone_table, "zones")) {
            std::cerr << "Unable to load zones table" << std::endl;
            return;
        }

        area_name_.setText(QString::fromStdString(zone_table[area_id][locale]));
    }

private:
    QHBoxLayout layout_;
    QLabel area_img_;
    QLabel area_name_;
};

#endif // AREA_ITEM_HPP
