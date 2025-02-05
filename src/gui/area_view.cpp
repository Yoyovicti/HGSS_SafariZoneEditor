#include "gui/area_view.hpp"

AreaView::AreaView(QWidget *parent) : QWidget(parent), layout_(this), view_3d_(this) {
    // std::filesystem::path maps_path("assets");
    // maps_path.append("maps");

    // std::filesystem::path img_path = maps_path / "Desert.png";
    // area_img_.setPixmap(QPixmap(img_path.string().c_str()));

    layout_.addWidget(&view_3d_);
}
