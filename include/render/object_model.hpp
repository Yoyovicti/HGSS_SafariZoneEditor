#ifndef OBJECTMODEL_HPP
#define OBJECTMODEL_HPP

#include "model.hpp"

class ObjectModel : public Model {
public:
    ObjectModel(const std::filesystem::path& model_dir, const QVector3D& pos, const QVector2D& dim) : Model(model_dir, convertPosition(pos, dim)), position_(pos), dimensions_(dim) {}

    QVector3D getPosition() {return position_;}

    void translate(QVector3D vector) {
        position_ += vector;

        transform_.translate(16.0f * vector);
        // QVector3D offset = convertPosition(position, dimensions_);
        // transform_(0, 3) = offset.x();
        // transform_(1, 3) = offset.y();
        // transform_(2, 3) = offset.z();
        // transform_.setColumn(3, QVector4D(convertPosition(position, dimensions_), 1.0));
        qDebug() << transform_;
    }

private:
    QVector3D position_;
    QVector2D dimensions_;

    QVector3D convertPosition(const QVector3D& pos, const QVector2D& dim) {
        return {
            16.0f * (pos.x() - (16.0f - dim.x() * 0.5f)),
            pos.y(),
            16.0f * (pos.z() - (15.0f + dim.y() * 0.5f))
        };
    }
};

#endif // OBJECTMODEL_HPP
