#pragma once

#include "repository_globals.hpp"
#include <QMimeData>

namespace dsrepository
{
class ComponentMimeData final : public QMimeData
{
public:
    ComponentMimeData(QList<ComponentData> components);
    static QString mimeType();
    QList<ComponentData> components() const;

    // QMimeData interface
public:
    bool hasFormat(const QString &mimetype) const override;
    QStringList formats() const override;

private:
    QList<ComponentData> components_;
};
} // namespace dsrepository
