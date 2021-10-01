#pragma once

#include <QString>

namespace dsrepository
{
struct DataBaseSetting
{
    QString name;
    QString hostName;
    QString userName;
    QString password;
    QString port;
    QString tableName = "Repository";
};
} // namespace dsrepository
