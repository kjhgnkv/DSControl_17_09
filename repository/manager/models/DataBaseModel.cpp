#include "DataBaseModel.hpp"

#include "repository/ComponentMimeData.hpp"
#include "repository/DataBaseController.hpp"
#include "tools.hpp"

#include <QMimeData>
#include <QSqlRecord>
#include <QUrl>

using namespace dsrepository::models;

DataBaseModel::DataBaseModel(QSqlDatabase &db, const QString &table, QObject* parent)
: QSqlTableModel(parent, db)
{
    setTable(table);
    setSort(0, Qt::AscendingOrder);
    setEditStrategy(QSqlTableModel::OnFieldChange);

    connect(this, &DataBaseModel::needUpdate, this, &DataBaseModel::select);
}

DataBaseModel::~DataBaseModel()
{
}

QVariant DataBaseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QSqlTableModel::headerData(section, orientation, role);
}

Qt::ItemFlags DataBaseModel::flags(const QModelIndex &index) const
{
    auto defaultFlags = QSqlTableModel::flags(index);

    if (!index.isValid())
    {
        return defaultFlags;
    }

    return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
}

bool DataBaseModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                                 const QModelIndex &parent)
{
    if (auto mimeData = dynamic_cast<const ComponentMimeData*>(data))
    {
        for (const auto &component : mimeData->components())
        {
            // TODO FIX
            // что делает эт строчка кода???

            // auto ret = JlCompress::compressDir("tmp.zip", component.path_, true);
            // if (!ret)
            // {
            //     return false;
            // }

            auto bytes = tools::fileBytes("tmp.zip");
            controller_->add(controller_->makeQuery(component.name_, component.version_, bytes));
        }
    }

    return QSqlTableModel::dropMimeData(data, action, row, column, parent);
}

Qt::DropActions DataBaseModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions DataBaseModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

QVariant dsrepository::models::DataBaseModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return {};
    }

    if (role == Role::DisplayRowRole)
    {
        auto name = QSqlTableModel::data(this->index(index.row(), Name), Qt::DisplayRole).toString();
        auto version = QSqlTableModel::data(this->index(index.row(), Version), Qt::DisplayRole).toString();

        return QVariant::fromValue<ComponentData>({name, version});
    }

    return QSqlTableModel::data(index, role);
}

bool dsrepository::models::DataBaseModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row,
                                                          int column, const QModelIndex &parent) const
{
    if (auto mimeData = dynamic_cast<const ComponentMimeData*>(data); mimeTypes().contains(mimeData->mimeType()))
    {
        return true;
    }
    else
    {
        return QSqlTableModel::canDropMimeData(data, action, row, column, parent);
    }
}

QMimeData* dsrepository::models::DataBaseModel::mimeData(const QModelIndexList &indexes) const
{
    QList<ComponentData> componentsData;
    for (const auto &index : indexes)
    {
        auto record = this->record(index.row());
        auto name = record.value("Name").toString();
        auto version = record.value("Version").toString();
        auto data = record.value("Data").toByteArray();

        ComponentData component = {{}, name, version, {}, data};
        if (!componentsData.contains(component))
        {
            componentsData.append(component);
        }
    }

    ComponentMimeData* mimeData = new ComponentMimeData(componentsData);

    return mimeData;
}

void DataBaseModel::setController(dsrepository::repository::DataBaseController* controller)
{
    controller_ = controller;
}

QStringList DataBaseModel::mimeTypes() const
{
    auto list = QSqlTableModel::mimeTypes();
    list.append(ComponentMimeData::mimeType());
    return list;
}
