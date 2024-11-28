#ifndef AREA_VIEW_HPP
#define AREA_VIEW_HPP

#include "locale_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>

#include <filesystem>
#include <array>
#include <iostream>

class AreaView : public QWidget {
private:
    static constexpr std::size_t SAFARI_OFFSET = 0xc13c;
    static constexpr std::size_t SLOT_SIZE = 0x7a;

    QGridLayout layout_;

    std::array<QImage, 12> areas_images_;
    std::array<QLabel, 6> image_labels_;
public:
    AreaView(QWidget* parent = nullptr);

    void loadData(const std::vector<unsigned char>& data);
};

#endif // AREA_VIEW_HPP
