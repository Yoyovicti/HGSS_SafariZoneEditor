#ifndef AREA_VIEW_HPP
#define AREA_VIEW_HPP

#include "day_counter_edit.hpp"
#include "area_editor/widget_3dview.hpp"
#include "area_editor/object_view.hpp"
#include "manager/save_data_manager.hpp"

#include <QScrollArea>

class AreaView : public QWidget {
    Q_OBJECT

public:
    AreaView(QWidget *parent = nullptr);

    void setDayCount(uint8_t count) {day_edit_.setText(QString::number(count));}
    void setSlot(const Slot& slot);
    void setModelDir(const std::filesystem::path& model_dir) {view_3d_.setModelDir(model_dir);}

private:
    QGridLayout layout_;

    QPushButton back_button_;
    QLabel day_label_;
    DayCounterEdit day_edit_;

    QLabel objects_label_;
    QPushButton obj_add_button_;

    Widget3DView view_3d_;

    QGridLayout* scroll_layout_;
    QScrollArea obj_area_;
    ObjectView object_view_;

signals:
    void backButtonReleased();
    void counterChanged(uint8_t c_id, uint8_t value);
};

#endif // AREA_VIEW_HPP
