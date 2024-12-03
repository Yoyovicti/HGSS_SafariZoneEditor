#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "manager/locale_manager.hpp"
#include "manager/config_manager.hpp"
#include "manager/save_file_manager.hpp"
#include "gui/file_menu.hpp"
#include "gui/options_menu.hpp"
#include "gui/area_view.hpp"
#include "gui/day_counters.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QMenuBar>
#include <QLabel>
#include <QFileDialog>

#include <string>
#include <filesystem>

class MainWindow : public QWidget {
private:
    QGridLayout layout_;
    QMenuBar menu_bar_;
    FileMenu file_menu_;
    OptionsMenu options_menu_;
    QLabel file_label_;

    AreaView area_view_;
    DayCounters day_counters_;

    std::vector<unsigned char> contents_;

    const std::string JSON_KEY = "main_window";

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private:
    void loadFileData();

private slots:
    void openFileDialog();
    void saveFileDialog();
    void updateLanguage(uint8_t locale);
};
#endif // MAIN_WINDOW_HPP
