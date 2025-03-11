#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "menu/file_menu.hpp"
#include "menu/options_menu.hpp"
#include "main/safari_layout.hpp"
#include "main/day_counters.hpp"
#include "main/area_selector.hpp"
#include "manager/save_data_manager.hpp"

#include <QMenuBar>
#include <QPushButton>
#include <QScrollArea>

class MainWindow : public QWidget {
public:
    inline static const std::string JSON_KEY = "main_window";

    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QGridLayout layout_;

    QMenuBar menu_bar_;
    FileMenu file_menu_;
    OptionsMenu options_menu_;

    QLabel file_label_;
    QLabel edit_area_label0_;
    QLabel edit_area_label1_;
    SafariLayout safari_layout_;
    DayCounters day_counters_;

    QPushButton edit_button_;
    QScrollArea area_scroll_;
    AreaSelector area_selector_;

    SaveDataManager save_data_manager_;

    uint8_t selected_area_;
    bool edit_mode_;

    void loadFileData();

private slots:
    void openFileDialog();
    void saveFileDialog();
    void areaClicked(uint8_t index);
    void updateLanguage(uint8_t locale);
    void updateCounters(uint8_t area_id, uint8_t value);
    void editButtonReleased();
    void highlightCounter(uint8_t& index, bool highlight);
    void updateSelectedArea(uint8_t index);
};
#endif // MAIN_WINDOW_HPP
