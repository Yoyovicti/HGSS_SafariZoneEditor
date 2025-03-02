#ifndef SAFARI_LAYOUT_VIEW_HPP
#define SAFARI_LAYOUT_VIEW_HPP

#include "safari_layout/area_label.hpp"
#include "safari_layout/block_popup.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QImage>
#include <QPixmap>

#include <array>

class SafariLayout : public QWidget {
    Q_OBJECT

public:
    SafariLayout(QWidget* parent = nullptr);

    void loadData(const std::array<Slot, SaveDataManager::N_SLOTS>& slot_data);
    void updateSlot(uint8_t area_slot, uint8_t area_type);
    void highlightSlot(uint8_t area_slot);

private:
    QGridLayout layout_;
    BlockPopup popup_;

    std::array<QImage, SaveDataManager::N_DAY_COUNTERS> areas_images_;
    std::array<AreaLabel, SaveDataManager::N_SLOTS> image_labels_;
    std::array<uint8_t, SaveDataManager::N_SLOTS> area_types_;
    std::array<std::array<uint8_t, SaveDataManager::BLOCK_TYPES>, SaveDataManager::N_SLOTS> block_counts_;

    uint8_t highlighted_slot_;
    bool data_loaded_;

signals:
    void areaHovered(uint8_t& index);
    void areaLeaveHover(uint8_t& index);
    void areaClicked(uint8_t index);

private slots:
    void labelEnterHover(size_t index);
    void labelLeaveHover(size_t index);
    void labelClicked(size_t index);
};

#endif // AREA_VIEW_HPP
