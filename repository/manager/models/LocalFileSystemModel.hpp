#pragma once

#include "tools.hpp"
#include <QFileSystemModel>

namespace dsrepository
{
class ConfigLoader;
namespace models
{
class LocalFileSystemModel : public QFileSystemModel
{
Q_OBJECT

public:
    enum Columns
    {
        FileName,
        Extension,
        FileVersion,
        Size
    };
    explicit LocalFileSystemModel(const QString &root, ConfigLoader* loader = nullptr, QObject* parent = nullptr);
    ~LocalFileSystemModel() override;

    void setLoader(ConfigLoader* loader);

private:
    QStringList getBundlesInDir(const QDir &dir) const;

private:
    QString root_;
    ConfigLoader* loader_;

public:
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                      const QModelIndex &parent) override;
    Qt::DropActions supportedDragActions() const override;
    Qt::DropActions supportedDropActions() const override;

    bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                         const QModelIndex &parent) const override;

    QStringList mimeTypes() const override;

    // QAbstractItemModel interface
public:
    QMimeData* mimeData(const QModelIndexList &indexes) const;
};

} // namespace models
} // namespace dsrepository
