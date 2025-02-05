#include "gui/block_popup.hpp"

BlockPopup::BlockPopup(QWidget* parent) : QWidget(parent), area_label_(this) {
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowTransparentForInput);

    layout_ = new QGridLayout(this);
    this->setLayout(layout_);

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json blocks_table;
    if(!locale_manager.getTable(&blocks_table, JSON_KEY)) {
        std::cerr << "Unable to load blocks table" << std::endl;
        return;
    }

    QFont font = area_label_.font();
    font.setBold(true);
    area_label_.setFont(font);
    layout_->addWidget(&area_label_, 0, 0, 1, 8);

    std::filesystem::path blocks_dir("assets");
    blocks_dir.append("objects");

    for(uint8_t i = 0; i < 12; i++) {
        std::string block_name = blocks_table[i][0];
        std::filesystem::path block_path = blocks_dir / (block_name + ".png");
        QPixmap block_image(block_path.string().c_str());
        block_images_[i] = block_image.scaled(20, 20);

        if(i % 3 == 0) {
            block_img_labels_[i / 3].setPixmap(block_images_[i]);
            block_count_labels_[i / 3].setText("0");

            layout_->addWidget(&block_img_labels_[i / 3], 1,  2 * i / 3, 1, 1);
            layout_->addWidget(&block_count_labels_[i / 3], 1, 2 * i / 3 + 1, 1, 1);
        }
    }
}

void BlockPopup::setCounters(const std::array<uint8_t, 4>& counters) {
    for(uint8_t i = 0; i < counters.size(); i++) {
        block_count_labels_[i].setText(QString::number(counters[i]));
    }
}

void BlockPopup::setAreaLabel(const std::string& area_name) {
    area_label_.setText(area_name.c_str());
}
