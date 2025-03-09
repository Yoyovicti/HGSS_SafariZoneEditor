#ifndef BLOCK_POPUP_HPP
#define BLOCK_POPUP_HPP

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

class BlockPopup : public QWidget {
public:
    BlockPopup(QWidget* parent = nullptr);

    void setCounters(const std::array<uint8_t, 4>& counters);
    void setAreaLabel(const std::string& area_name);

private:
    const std::string JSON_KEY = "objects";

    QGridLayout* layout_;

    std::array<QPixmap, 12> block_images_;

    QLabel area_label_;
    std::array<QLabel, 4> block_img_labels_;
    std::array<QLabel, 4> block_count_labels_;
};

#endif // BLOCK_POPUP_HPP
