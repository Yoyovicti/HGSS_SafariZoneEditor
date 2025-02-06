#ifndef AREA_VIEW_HPP
#define AREA_VIEW_HPP

#include "gui/widget_3dview.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>

#include <filesystem>

class AreaView : public QWidget {
    Q_OBJECT
public:
    AreaView(QWidget *parent = nullptr);

    void setModelDir(const std::filesystem::path& model_dir) {view_3d_.setModelDir(model_dir);}

private:
    QGridLayout layout_;

    QPushButton back_button_;
    Widget3DView view_3d_;

    // QLabel area_img_;

signals:
    void backButtonReleased();

};

#endif // AREA_VIEW_HPP
