#pragma once

#include <QSqlTableModel>

namespace dsrepository
{
namespace repository
{
class DataBaseController;
}

namespace models
{
class DataBaseModel : public QSqlTableModel
{
Q_OBJECT
public:
    enum
    {
        Name = 0,
        Version,
        Data,
        Size
    };

    enum Role
    {
        DisplayRowRole = Qt::ItemDataRole::UserRole
    };

    explicit DataBaseModel(QSqlDatabase &db, const QString &table = "", QObject* parent = nullptr);
    ~DataBaseModel() override;

    // QAbstractItemModel interface
public:
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                      const QModelIndex &parent) override;
    Qt::DropActions supportedDropActions() const override;
    Qt::DropActions supportedDragActions() const override;

    bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                         const QModelIndex &parent) const override;
    QMimeData* mimeData(const QModelIndexList &indexes) const override;

    void setController(repository::DataBaseController* controller);
    QStringList mimeTypes() const;

signals:
    void needUpdate();

private:
    repository::DataBaseController* controller_;
};

} // namespace models
} // namespace dsrepository
