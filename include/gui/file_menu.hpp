#ifndef FILE_MENU_HPP
#define FILE_MENU_HPP

#include "manager/locale_manager.hpp"
#include "manager/config_manager.hpp"

#include <QMenu>

#include <iostream>

class FileMenu : public QMenu {
    Q_OBJECT

private:
    const std::string JSON_KEY = "file";

public:
    FileMenu(QWidget *parent = nullptr);
    ~FileMenu();

    void updateLanguage(uint8_t& locale);

signals:
    void openFileClicked();
    void saveAsFileClicked();
};

#endif
