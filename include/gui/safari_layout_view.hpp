#ifndef SAFARI_LAYOUT_VIEW_HPP
#define SAFARI_LAYOUT_VIEW_HPP

// #include "manager/save_file_manager.hpp"
#include "gui/area_label.hpp"
#include "gui/block_popup.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>

#include <array>

class SafariLayoutView : public QWidget {
    Q_OBJECT

private:
    QGridLayout layout_;
    BlockPopup popup_;

    std::array<QImage, SaveDataManager::N_DAY_COUNTERS> areas_images_;
    std::array<AreaLabel, SaveDataManager::N_SLOTS> image_labels_;
    std::array<uint8_t, SaveDataManager::N_SLOTS> area_types_;
    std::array<std::array<uint8_t, SaveDataManager::BLOCK_TYPES>, SaveDataManager::N_SLOTS> block_counts_;

    bool data_loaded_;

public:
    SafariLayoutView(QWidget* parent = nullptr);

    void loadData(const std::array<Slot, SaveDataManager::N_SLOTS>& slot_data);

signals:
    void areaHovered(uint8_t& index);
    void areaLeaveHover(uint8_t& index);
    void areaClicked(size_t index);

private slots:
    void labelEnterHover(size_t index);
    void labelLeaveHover(size_t index);
    void labelClicked(size_t index);
};

#endif // AREA_VIEW_HPP
