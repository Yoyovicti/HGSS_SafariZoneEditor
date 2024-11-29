#include "block_popup.hpp"

#include <QLabel>
#include <QStyle>
#include <iostream>

BlockPopup::BlockPopup(QWidget* parent) : QWidget(parent) {
    layout_ = new QGridLayout(this);
    this->setLayout(layout_);

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json blocks_table;
    if(!locale_manager.getTable(&blocks_table, JSON_KEY)) {
        std::cerr << "Unable to load blocks table" << std::endl;
        return;
    }

    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_ShowWithoutActivating);

    std::filesystem::path blocks_dir("assets");
    blocks_dir.append("safari_blocks");

    for(int i = 0; i < 12; i++) {
        std::string block_name = blocks_table[i][0];
        std::filesystem::path block_path = blocks_dir / (block_name + ".png");
        QPixmap block_image(block_path.string().c_str());
        block_images_[i] = block_image.scaled(25, 25);

        if(i % 3 == 0) {
            block_img_labels_[i / 3].setPixmap(block_images_[i]);
            block_count_labels_[i / 3].setText("0");

            layout_->addWidget(&block_img_labels_[i / 3], i / 3, 0, 1, 1);
            layout_->addWidget(&block_count_labels_[i / 3], i / 3, 1, 1, 1);
        }
    }
}

void BlockPopup::setCounters(const std::array<uint8_t, 4>& counters) {
    for(uint8_t i = 0; i < counters.size(); i++) {
        block_count_labels_[i].setText(QString::number(counters[i]));
    }
}
