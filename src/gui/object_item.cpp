#include "gui/object_item.hpp"

#include "manager/locale_manager.hpp"

#include <QFontDatabase>
#include <QPainter>

ObjectItem::ObjectItem(QWidget *parent) : QWidget(parent), layout_(this) {
    setObjectName("object_item");
    setAttribute(Qt::WA_StyledBackground, true);

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

    const QColor& text_color = object_label_.palette().color(QPalette::Text);
    move_button_.setIcon(loadAndRecolorIcon("assets/icons/drag-arrow-icon.png", text_color));
    delete_button_.setIcon(loadAndRecolorIcon("assets/icons/delete-icon.png", text_color));

    layout_.addWidget(&icon_label_, 0, 0, 2, 2);
    layout_.addWidget(&object_label_, 0, 2, 1, 4);
    layout_.addWidget(&x_label_, 1, 2, 1, 1);
    layout_.addWidget(&cx_label_, 1, 3, 1, 1);
    layout_.addWidget(&y_label_, 1, 4, 1, 1);
    layout_.addWidget(&cy_label_, 1, 5, 1, 1);
    layout_.addWidget(&move_button_, 0, 6, 1, 1);
    layout_.addWidget(&delete_button_, 1, 6, 1, 1);
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
}

void ObjectItem::enterEvent(QEnterEvent *event) {
    setStyleSheet("#object_item { background-color: rgba(100, 100, 255, 60); }");
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
