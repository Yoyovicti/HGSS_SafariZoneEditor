#ifndef LANG_MENU_HPP
#define LANG_MENU_HPP

#include <QMenu>
#include <QActionGroup>

class LangMenu : public QMenu {
    Q_OBJECT

public:
    inline static const std::string JSON_KEY = "locale";

    LangMenu(QWidget *parent = nullptr);
    ~LangMenu();

    void updateLanguage(uint8_t& locale);

private:
    QActionGroup action_group_;

signals:
    void languageChangeClicked(uint8_t locale); 
};

#endif
