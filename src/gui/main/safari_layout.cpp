#include "safari_layout.hpp"

#include "manager/locale_manager.hpp"
#include "manager/config_manager.hpp"

SafariLayout::SafariLayout(QWidget* parent) : QWidget(parent), layout_(this), highlighted_slot_(-1), data_loaded_(false) {
    std::filesystem::path areas_dir("assets");
    areas_dir.append("icons");

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json zone_table;
    if(!locale_manager.getTable(&zone_table, "zones")) {
        std::cerr << "Unable to load zones table" << std::endl;
        return;
    }

    std::filesystem::path area_path(areas_dir);
    for(uint8_t i = 0; i < zone_table.size(); i++) {
        std::string area = zone_table[i][0];
        std::filesystem::path area_path = areas_dir / (area + ".png");
        QImage area_image(area_path.string().c_str());
        areas_images_[i] = area_image.scaled(area_image.size() * 2);
    }

    for(uint8_t i = 0; i < 6; i++) {
        block_counts_[i].fill(0);
        image_labels_[i].setPixmap(QPixmap::fromImage(areas_images_[i]));
        image_labels_[i].setEnabled(false);
        layout_.addWidget(&image_labels_[i], (i / 3) * 6, (i % 3) * 6, 6, 6);

        QObject::connect(&image_labels_[i], &AreaLabel::enterHover, this, [this, i](){labelEnterHover(i);});
        QObject::connect(&image_labels_[i], &AreaLabel::leaveHover, this, [this, i](){labelLeaveHover(i);});
        QObject::connect(&image_labels_[i], &AreaLabel::clicked, this, [this, i](){labelClicked(i);});
    }
}

void SafariLayout::loadData(const std::array<Slot, SaveDataManager::N_SLOTS>& slot_data) {
    for(uint8_t i = 0; i < slot_data.size(); i++) {
        area_types_[i] = slot_data[i].area_type_;
        image_labels_[i].setPixmap(QPixmap::fromImage(areas_images_[area_types_[i]]));
        image_labels_[i].setEnabled(true);

        block_counts_[i].fill(0);
        uint8_t obj_count = slot_data[i].object_count_;
        for(uint8_t j = 0; j < obj_count; j++) {
            uint8_t item_type = slot_data[i].objects_[j].id_;
            if(item_type < SaveDataManager::BLOCK_TYPES) {
                block_counts_[i][item_type]++;
            }
        }
    }

    data_loaded_ = true;
}

void SafariLayout::updateSlot(uint8_t area_slot, uint8_t area_type) {
    area_types_[area_slot] = area_type;
    image_labels_[area_slot].setPixmap(QPixmap::fromImage(areas_images_[area_type]));
    popup_.hide();
    block_counts_[area_slot].fill(0);
}

void SafariLayout::highlightSlot(uint8_t area_slot) {
    if(highlighted_slot_ < image_labels_.size())
        image_labels_[highlighted_slot_].setSelected(false);
    if(area_slot >= image_labels_.size()) return;

    image_labels_[area_slot].setSelected(true);
    highlighted_slot_ = area_slot;
}

void SafariLayout::labelEnterHover(uint8_t index) {
    if(!data_loaded_)
        return;

    std::array<uint8_t, SaveDataManager::BLOCK_TYPES> area_block_counts = block_counts_[index];
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

    emit areaEnterHover(area_types_[index]);
    popup_.setCounters(type_counts);
    popup_.setAreaLabel(zone_table[area_types_[index]][locale]);
    QPoint popup_pos = image_labels_[index].mapToGlobal(QPoint(0, 0));
    popup_.move(popup_pos);

    // Force size to adapt to contents (seems required when using Qt::WindowTransparentForInput with X11)
    popup_.setFixedSize(popup_.sizeHint());
    popup_.show();
}

void SafariLayout::labelLeaveHover(uint8_t index) {
    if(!data_loaded_)
        return;

    emit areaLeaveHover(area_types_[index]);
    popup_.hide();
}

void SafariLayout::labelClicked(uint8_t index) {
    emit areaClicked(index);
}
