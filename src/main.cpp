#include "main_window.hpp"

#include <QApplication>
#include <QScreen>

#include <sstream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;

    std::ostringstream window_title;
    window_title << "HGSS Safari Zone editor " << VERSION_MAJOR << "." << VERSION_MINOR;
    window.setWindowTitle(QString::fromStdString(window_title.str()));

    QScreen* screen = QApplication::primaryScreen();
    QRect screen_geometry = screen->availableGeometry();
    int x = (screen_geometry.width() - window.width()) / 2;
    int y = (screen_geometry.height() - window.height()) / 2;
    window.move(x, y);

    window.show();
    return app.exec();
}
