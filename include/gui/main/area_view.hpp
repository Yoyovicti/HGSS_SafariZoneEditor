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
    AreaView(SaveDataManager* save_data_manager_, QWidget *parent = nullptr);

    void setDayCount(uint8_t count) {day_edit_.setText(QString::number(count));}

    void setSlot(uint8_t slot_index);
    void setModelDir(const std::filesystem::path& model_dir) {view_3d_.setModelDir(model_dir);}

    void keyReleaseEvent(QKeyEvent* event) override;

    void updateLanguage(uint8_t locale);
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

    SaveDataManager* save_data_manager_;
    uint8_t selected_slot_;
    uint8_t selected_object_;
    bool edit_mode_;

signals:
    void backButtonReleased();
    void counterChanged(uint8_t c_id, uint8_t value);

private slots:
    void enterItemHover(uint8_t i);
    void leaveItemHover(uint8_t i);
};

#endif // AREA_VIEW_HPP
