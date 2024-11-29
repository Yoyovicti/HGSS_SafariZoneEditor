#ifndef AREA_VIEW_HPP
#define AREA_VIEW_HPP

#include "locale_manager.hpp"
#include "hover_label.hpp"
#include "block_popup.hpp"

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
    BlockPopup popup_;

    std::array<QImage, 12> areas_images_;
    std::array<HoverLabel, 6> image_labels_;
    std::array<std::array<uint8_t, 12>, 6> block_counts_;
public:
    AreaView(QWidget* parent = nullptr);

    void loadData(const std::vector<unsigned char>& data);

private slots:
    void labelEnterHover(size_t index);
    void labelLeaveHover(size_t index);
};

#endif // AREA_VIEW_HPP
