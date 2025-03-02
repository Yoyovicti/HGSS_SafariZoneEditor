#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "gui/file_menu.hpp"
#include "gui/options_menu.hpp"
#include "gui/safari_layout_view.hpp"
#include "gui/area_view.hpp"
#include "gui/widget_3dview.hpp"
#include "gui/day_counters.hpp"
#include "gui/area_selector.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QMenuBar>
#include <QLabel>
#include <QFileDialog>
#include <QPushButton>

#include <string>

class MainWindow : public QWidget {
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    QGridLayout layout_;

    QMenuBar menu_bar_;
    FileMenu file_menu_;
    OptionsMenu options_menu_;

    QLabel file_label_;
    SafariLayoutView layout_view_;
    DayCounters day_counters_;
    AreaView area_view_;

    QPushButton edit_button_;
    QScrollArea area_scroll_;
    AreaSelector area_selector_;

    SaveDataManager save_data_manager_;

    const std::string JSON_KEY = "main_window";

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
    void updateSelectedArea(uint8_t index);
};
#endif // MAIN_WINDOW_HPP
