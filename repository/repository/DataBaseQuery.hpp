#pragma once

#include <QString>

namespace dsrepository
{
struct DataBaseQuery
{
    QString name;
    QString version;
    QByteArray data;

    bool isValid() const
    {
        if (name.isEmpty() || version.isEmpty())
        {
            return false;
        }
        return true;
    }
};
} // namespace dsrepository
