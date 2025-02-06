#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "gui/file_menu.hpp"
#include "gui/options_menu.hpp"
#include "gui/safari_layout_view.hpp"
#include "gui/area_view.hpp"
#include "gui/widget_3dview.hpp"
#include "gui/day_counters.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QMenuBar>
#include <QLabel>
#include <QFileDialog>

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

    SaveDataManager save_data_manager_;

    const std::string JSON_KEY = "main_window";

    void loadFileData();

private slots:
    void openFileDialog();
    void saveFileDialog();
    void updateLanguage(uint8_t locale);
    void enterAreaViewer(size_t index);
    void exitAreaViewer();
};
#endif // MAIN_WINDOW_HPP
