#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include "file_menu.hpp"
#include "lang_menu.hpp"
#include "day_counters.hpp"
#include "locale_manager.hpp"
#include "config_manager.hpp"
#include "save_file_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QMenuBar>
#include <QLabel>
#include <QFileDialog>

#include <string>
#include <filesystem>

class MainWindow : public QWidget
{
    Q_OBJECT
private:
    QGridLayout layout_;
    QMenuBar menu_bar_;
    FileMenu file_menu_;
    LangMenu lang_menu_;
    QLabel file_label_;
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
