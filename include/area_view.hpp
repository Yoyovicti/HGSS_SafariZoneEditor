#ifndef AREA_VIEW_HPP
#define AREA_VIEW_HPP

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>

#include <filesystem>
#include <array>

class AreaView : public QWidget {
    QGridLayout layout_;

    std::array<QImage, 12> areas_images_;
    std::array<QLabel, 6> image_labels_;
public:
    AreaView(QWidget* parent = nullptr);
};

#endif // AREA_VIEW_HPP
