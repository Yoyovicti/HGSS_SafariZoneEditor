#include "area_view.hpp"

AreaView::AreaView(QWidget* parent) : QWidget(parent), layout_(this), data_loaded_(false) {
    std::filesystem::path areas_dir("assets");
    areas_dir.append("safari_icons");

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json zone_table;
    if(!locale_manager.getTable(&zone_table, "zones")) {
        std::cerr << "Unable to load zones table" << std::endl;
        return;
    }

    std::filesystem::path area_path(areas_dir);
    for(size_t i = 0; i < zone_table.size(); i++) {
        std::string area = zone_table[i][0];
        std::filesystem::path area_path = areas_dir / (area + ".png");
        QImage area_image(area_path.string().c_str());
        areas_images_[i] = area_image.scaled(area_image.size() * 2);
    }

    for(size_t i = 0; i < 6; i++) {
        block_counts_[i].fill(0);
        image_labels_[i].setPixmap(QPixmap::fromImage(areas_images_[i]));
        image_labels_[i].setEnabled(false);
        layout_.addWidget(&image_labels_[i], (i / 3) * 6, (i % 3) * 6, 6, 6);

        QObject::connect(&image_labels_[i], &HoverLabel::enterHover, this, [this, i](){labelEnterHover(i);});
        QObject::connect(&image_labels_[i], &HoverLabel::leaveHover, this, [this, i](){labelLeaveHover(i);});
    }
}

void AreaView::loadData(const std::vector<unsigned char>& data) {
    for(uint8_t i = 0; i < 6; i++) {
        area_types_[i] = data[SAFARI_OFFSET + i * SLOT_SIZE];
        image_labels_[i].setPixmap(QPixmap::fromImage(areas_images_[area_types_[i]]));
        image_labels_[i].setEnabled(true);

        block_counts_[i].fill(0);
        uint8_t block_count = data[SAFARI_OFFSET + i * SLOT_SIZE + 1];
        for(uint8_t j = 0; j < block_count; j++) {
            uint8_t item_type = data[SAFARI_OFFSET + i * SLOT_SIZE + 2 + j * 4];
            if(item_type < 12) {
                block_counts_[i][item_type]++;
            }
        }
    }

    data_loaded_ = true;
}

void AreaView::labelEnterHover(size_t index) {
    if(!data_loaded_)
        return;

    std::array<uint8_t, 12> area_block_counts = block_counts_[index];
    std::array<uint8_t, 4> type_counts;
    for(uint8_t i = 0; i < 4; i++) {
        auto it = area_block_counts.begin() + 3 * i;
        type_counts[i] = std::accumulate(it, it + 3, 0);
    }

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json zone_table;
    if(!locale_manager.getTable(&zone_table, "zones")) {
        std::cerr << "Unable to load zones table" << std::endl;
        return;
    }

    ConfigManager& config_manager = ConfigManager::getInstance();
    uint8_t locale = config_manager.getLocale();

    emit areaHovered(area_types_[index]);
    popup_.setCounters(type_counts);
    popup_.setAreaLabel(zone_table[area_types_[index]][locale]);
    QPoint popup_pos = image_labels_[index].mapToGlobal(QPoint(0, 0));
    popup_.move(popup_pos);
    popup_.show();
}

void AreaView::labelLeaveHover(size_t index) {
    if(!data_loaded_)
        return;

    emit areaLeaveHover(area_types_[index]);
    popup_.hide();
}
