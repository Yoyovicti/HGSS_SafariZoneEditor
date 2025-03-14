#ifndef UTILS_HPP
#define UTILS_HPP

#include <QIcon>
#include <QPainter>

QIcon loadAndRecolorIcon(const QString& img_path, const QColor& color) {
    QPixmap pixmap(img_path);
    QPainter painter(&pixmap);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pixmap.rect(), color);
    painter.end();
    return QIcon(pixmap);
}

#endif // UTILS_HPP
