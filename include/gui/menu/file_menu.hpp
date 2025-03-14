#ifndef FILE_MENU_HPP
#define FILE_MENU_HPP

#include <QMenu>

class FileMenu : public QMenu {
    Q_OBJECT

public:
    inline static const std::string JSON_KEY = "file";

    FileMenu(QWidget *parent = nullptr);
    ~FileMenu();

    void updateLanguage(uint8_t& locale);

signals:
    void openFileClicked();
    void saveAsFileClicked();
};

#endif
