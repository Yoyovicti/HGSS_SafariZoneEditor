#ifndef AREA_VIEW_HPP
#define AREA_VIEW_HPP

#include "gui/widget_3dview.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>

#include <filesystem>

class AreaView : public QWidget {
private:
    QGridLayout layout_;

    Widget3DView view_3d_;

    // QLabel area_img_;

public:
    AreaView(QWidget *parent = nullptr);
};

#endif // AREA_VIEW_HPP
