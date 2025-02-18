#include "gui/object_item.hpp"

#include "manager/locale_manager.hpp"

#include <QFontDatabase>
#include <QPainter>

ObjectItem::ObjectItem(QWidget *parent) : QWidget(parent), layout_(this) {
    // color_effect_.setColor(Qt::white);

    int max_icon_size = 50;
    icon_label_.setFixedSize(max_icon_size, max_icon_size);
    icon_label_.setAlignment(Qt::AlignCenter);

    x_label_.setText("X");
    y_label_.setText("Y");

    QFont custom_font;
    custom_font.setBold(true);
    custom_font.setPointSize(16);
    std::string font_path = "assets/fonts/pasti-font/PastiOblique-7B0wK.otf";
    int font_id = QFontDatabase::addApplicationFont(font_path.c_str());
    if(font_id < 0) {
        std::cerr << "ObjectItem::ObjectItem unable to load font: " << font_path << std::endl;
    } else {
        QString font_family = QFontDatabase::applicationFontFamilies(font_id)[0];
        custom_font.setFamily(font_family);
    }

    x_label_.setFont(custom_font);
    x_label_.setStyleSheet("color: slateblue");
    y_label_.setFont(custom_font);
    y_label_.setStyleSheet("color: indianred");

    // QIcon move_icon;
    // move_icon
    // QIcon m_icon()

    // QGraphicsColorizeEffect effect = move_button_.graphicsEffect();
    // effect.setColor(object_label_.palette().color(QPalette::Text));
    // effect.setColor(Qt::white);
    // move_icon.setIc
    const QColor& text_color = object_label_.palette().color(QPalette::Text);
    move_button_.setIcon(loadAndRecolorIcon("assets/icons/drag-arrow-icon.png", text_color));
    delete_button_.setIcon(loadAndRecolorIcon("assets/icons/delete-icon.png", text_color));
    // move_button_.setGraphicsEffect(&color_effect_);

    // QIcon delete_icon("assets/icons/delete-icon.png");
    // delete_button_.setIcon(delete_icon);

    layout_.addWidget(&icon_label_, 0, 0, 2, 2);
    layout_.addWidget(&object_label_, 0, 2, 1, 4);
    layout_.addWidget(&x_label_, 1, 2, 1, 1);
    layout_.addWidget(&cx_label_, 1, 3, 1, 1);
    layout_.addWidget(&y_label_, 1, 4, 1, 1);
    layout_.addWidget(&cy_label_, 1, 5, 1, 1);
    layout_.addWidget(&move_button_, 0, 6, 1, 1);
    layout_.addWidget(&delete_button_, 1, 6, 1, 1);

    // setLayout(&layout_);
}

void ObjectItem::setObject(const Object& object) {
    std::filesystem::path obj_dir("assets");
    obj_dir.append("objects");

    LocaleManager& locale_manager = LocaleManager::getInstance();
    json obj_table;
    if(!locale_manager.getTable(&obj_table, "objects")) {
        std::cerr << "Unable to load objects table" << std::endl;
        return;
    }

    std::string obj_name = obj_table[object.id_][0];
    object_label_.setText(obj_name.c_str());

    std::filesystem::path obj_path(obj_dir / (obj_name + ".png"));
    QImage obj_image(obj_path.string().c_str());
    icon_label_.setPixmap(QPixmap::fromImage(obj_image.scaled(50, 50, Qt::KeepAspectRatio)));

    cx_label_.setText(QString::number(object.x_));
    cy_label_.setText(QString::number(object.y_));




    // std::filesystem::path area_path(areas_dir);
    // for(size_t i = 0; i < obj_table.size(); i++) {
    //     std::string area = obj_table[i][0];
    //     std::filesystem::path area_path = areas_dir / (area + ".png");
    //     QImage area_image(area_path.string().c_str());
    //     areas_images_[i] = area_image.scaled(area_image.size() * 2);
    // }

    // for(size_t i = 0; i < 6; i++) {
    //     block_counts_[i].fill(0);
    //     image_labels_[i].setPixmap(QPixmap::fromImage(areas_images_[i]));
    //     image_labels_[i].setEnabled(false);
    //     layout_.addWidget(&image_labels_[i], (i / 3) * 6, (i % 3) * 6, 6, 6);

    //     QObject::connect(&image_labels_[i], &AreaLabel::enterHover, this, [this, i](){labelEnterHover(i);});
    //     QObject::connect(&image_labels_[i], &AreaLabel::leaveHover, this, [this, i](){labelLeaveHover(i);});
    //     QObject::connect(&image_labels_[i], &AreaLabel::clicked, this, [this, i](){labelClicked(i);});
    // }
}

void ObjectItem::enterEvent(QEnterEvent *event) {
    setStyleSheet("QWidget { background-color: rgba(100, 100, 255, 60); }"
                  "QLabel { background: transparent; }");
    emit enterHover();

}

void ObjectItem::leaveEvent(QEvent *event) {
    setStyleSheet("");
    emit leaveHover();
}

// void ObjectItem::mousePressEvent(QMouseEvent *event) {

// }

QIcon ObjectItem::loadAndRecolorIcon(const QString& img_path, const QColor& color) {
    QPixmap pixmap(img_path);
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    painter.end();
    return QIcon(pixmap);
}
