#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "menu/file_menu.hpp"
#include "menu/options_menu.hpp"
#include "main/safari_layout.hpp"
#include "main/area_view.hpp"
#include "main/day_counters.hpp"
#include "main/area_selector.hpp"
#include "manager/save_data_manager.hpp"

#include <QMenuBar>

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
    SafariLayout safari_layout_;
    DayCounters day_counters_;
    AreaView area_view_;

    QPushButton edit_button_;
    QScrollArea area_scroll_;
    AreaSelector area_selector_;

    SaveDataManager save_data_manager_;

    uint8_t selected_area_;
    bool edit_mode_;

    void loadFileData();
    void enterAreaViewer(uint8_t index);

private slots:
    void openFileDialog();
    void saveFileDialog();
    void areaClicked(uint8_t index);
    void exitAreaViewer();
    void updateLanguage(uint8_t locale);
    void updateCounters(uint8_t area_id, uint8_t value);
    void editButtonReleased();
    void highlightCounter(uint8_t& index, bool highlight);
    void updateSelectedArea(uint8_t index);
};
#endif // MAIN_WINDOW_HPP
