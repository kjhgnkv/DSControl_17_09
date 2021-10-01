#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>

namespace dsrepository
{
enum class ModelType
{
    FileSystem,
    DataBase,
    None,
};

struct ComponentData
{
    QString path_ {};
    QString name_ {};
    QString version_ {};
    QJsonObject info_ {};
    QByteArray data_ {};

    bool operator==(const ComponentData &rhs)
    {
        return rhs.path_ == this->path_ && this->name_ == rhs.name_ && this->version_ == rhs.version_;
    }

};
} // namespace dsrepository
Q_DECLARE_METATYPE(dsrepository::ComponentData)
