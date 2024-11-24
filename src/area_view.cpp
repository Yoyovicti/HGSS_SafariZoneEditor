#include "area_view.hpp"

AreaView::AreaView(QWidget* parent) : QWidget(parent), layout_(this) {
    std::filesystem::path areas_dir("assets");
    areas_dir.append("safari_icons");

    size_t i = 0;
    for(const auto& entry : std::filesystem::directory_iterator(areas_dir)) {
        QImage area_image(entry.path().string().c_str());
        areas_images_[i] = area_image.scaled(area_image.size() * 2);
        i++;
    }

    for(i = 0; i < 6; i++) {
        image_labels_[i].setPixmap(QPixmap::fromImage(areas_images_[i]));
        layout_.addWidget(&image_labels_[i], (i / 3) * 6, (i % 3) * 6, 6, 6);
    }
}
