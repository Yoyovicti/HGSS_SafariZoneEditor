#include "area_view.hpp"

AreaView::AreaView(QWidget* parent) : QWidget(parent), layout_(this) {
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
        i++;
    }

    for(size_t i = 0; i < 6; i++) {
        image_labels_[i].setPixmap(QPixmap::fromImage(areas_images_[i]));
        layout_.addWidget(&image_labels_[i], (i / 3) * 6, (i % 3) * 6, 6, 6);
    }
}

void AreaView::loadData(const std::vector<unsigned char>& data) {
    LocaleManager& locale_manager = LocaleManager::getInstance();
    json zone_table;
    if(!locale_manager.getTable(&zone_table, "zones")) {
        std::cerr << "Unable to load zones table" << std::endl;
        return;
    }

    for(size_t i = 0; i < 6; i++) {
        unsigned char area_type = data[SAFARI_OFFSET + i * SLOT_SIZE];
        image_labels_[i].setPixmap(QPixmap::fromImage(areas_images_[area_type]));

        std::cout << std::hex << SAFARI_OFFSET + i * SLOT_SIZE << std::endl;
        std::cout << std::dec << static_cast<int>(data[SAFARI_OFFSET + i * SLOT_SIZE + 1]) << std::endl;
    }
}
