#ifndef AREA_VIEW_HPP
#define AREA_VIEW_HPP

#include "gui/widget_3dview.hpp"
#include "gui/object_view.hpp"
#include "manager/save_data_manager.hpp"

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

#include <array>
#include <filesystem>
#include <sstream>

class AreaView : public QWidget {
    Q_OBJECT
public:
    AreaView(QWidget *parent = nullptr);

    void setSlot(const Slot& slot);
    void setModelDir(const std::filesystem::path& model_dir) {view_3d_.setModelDir(model_dir);}

private:
    QGridLayout layout_;

    QPushButton back_button_;
    QLabel objects_label_;

    Widget3DView view_3d_;

    // QWidget scroll_container_;
    // QGridLayout scroll_layout_;
    QGridLayout* scroll_layout_;
    QScrollArea obj_area_;
    ObjectView object_view_;

    // QLabel area_img_;

signals:
    void backButtonReleased();

};

#endif // AREA_VIEW_HPP
