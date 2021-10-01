#include "LocalFileSystemModel.hpp"

// TODO
// error JlCompress
// #include "3rdparty/quazip/quazip/JlCompress.h"
#include "manager/ConfigLoader.hpp"
#include "repository/ComponentMimeData.hpp"
#include <QJsonObject>

using namespace dsrepository;
using namespace dsrepository::models;

LocalFileSystemModel::LocalFileSystemModel(const QString &root, ConfigLoader* loader, QObject* parent)
: QFileSystemModel(parent)
, root_ {root}
, loader_ {loader}
{
    setReadOnly(false);
    setRootPath(root_);

    //Now we use bandles, so test and remove commented code if it will not be necessary
    //setNameFilters({"*.so", "*.dll", "*.xml", "*.ui"});
    //setNameFilterDisables(false);
    // 
    // Only directories(bandles)
    setFilter(QDir::Dirs|QDir::Drives|QDir::NoDotAndDotDot|QDir::AllDirs);
}

LocalFileSystemModel::~LocalFileSystemModel()
{
}

void LocalFileSystemModel::setLoader(ConfigLoader* loader)
{
    if (loader)
    {
        loader_ = loader;
    }
}

int LocalFileSystemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return Columns::Size;
}

QVariant LocalFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0)
    {
        return {};
    }
    if (!loader_)
    {
        return {};
    }

    QVariant retval {};

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
            case Columns::FileName:
            {
                retval = fileInfo(index).isFile() ? fileInfo(index).baseName() : QFileSystemModel::data(index, role);
                break;
            }
            case Columns::Extension:
            {
                if (fileInfo(index).isFile())
                {
                    return fileInfo(index).suffix();
                }
                break;
            }
            case Columns::FileVersion:
            {
                //QString name;
                /*if (fileInfo(index).isDir())
                {
                    name = tools::componentToConfig(filePath(index));
                }

                if (name.isEmpty())
                {
                    return retval;
                }*/
                if (!fileInfo(index).isDir())
                {
                    return retval;
                }
                
                auto components = loader_->components();
                for (auto &filePath : getBundlesInDir({filePath(index)}))
                {
                    //qDebug() << "BUNDLE:" << filePath;
                    if (auto found = components.find(filePath); found != components.end())
                    {
                        auto component = found->second;
                        if (component.info_["moduleType"] == "general")
                        {
                            retval = component.version_;
                        }
                        break;
                    }
                }

                break;
            }
        }
    }

    return retval;
}

QVariant LocalFileSystemModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    switch (section)
    {
        case Columns::FileVersion:
        {
            return "Version";
        }
        case Columns::Extension:
        {
            return "Ext";
        }
        default:
        {
            return QFileSystemModel::headerData(section, orientation, role);
        }
    }
}

Qt::ItemFlags LocalFileSystemModel::flags(const QModelIndex &index) const
{
    auto defaultFlags = QFileSystemModel::flags(index);

    if (!index.isValid())
    {
        return defaultFlags;
    }

    if (auto info = fileInfo(index); info.isDir())
    {
        return Qt::ItemIsDropEnabled | Qt::ItemIsDragEnabled | defaultFlags;
    }

    return defaultFlags;
}

bool LocalFileSystemModel::dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                                        const QModelIndex &parent)
{
    if (auto mimeData = dynamic_cast<const ComponentMimeData*>(data))
    {
        for (const auto &component : mimeData->components())
        {
            auto componentDirPath = tools::createFilePathToComponentExtraction(filePath(parent), component);
            tools::createFile("tmp.zip", component.data_);
            // JlCompress::extractDir("tmp.zip", componentDirPath);
        }

        return true;
    }

    return QFileSystemModel::dropMimeData(data, action, row, column, parent);
}

Qt::DropActions LocalFileSystemModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions LocalFileSystemModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

bool LocalFileSystemModel::canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column,
                                           const QModelIndex &parent) const
{
    if (auto mimeData = dynamic_cast<const ComponentMimeData*>(data); mimeTypes().contains(mimeData->mimeType()))
    {
        return true;
    }
    else
    {
        return QFileSystemModel::canDropMimeData(data, action, row, column, parent);
    }
}

QStringList LocalFileSystemModel::mimeTypes() const
{
    auto list = QFileSystemModel::mimeTypes();
    list.append(ComponentMimeData::mimeType());
    return list;
}

QMimeData* LocalFileSystemModel::mimeData(const QModelIndexList &indexes) const
{
    auto data = QFileSystemModel::mimeData(indexes);

    QList<ComponentData> componentsData;
    for (const auto &index : indexes)
    {
        QString version;
        QJsonObject info;
        //auto name = tools::componentToConfig(filePath(index));
        auto components = loader_->components();
        for (auto &name : getBundlesInDir({filePath(index)}))
        {
            if (auto found = components.find(name); found != components.end())
            {
                auto component = found->second;
                version = component.version_;
                info = component.info_;
                if (info["moduleType"] != "general")
                {
                    continue;
                }
            }
            else
            {
                continue;
            }

            ComponentData component = {name, fileInfo(index).dir().dirName(), version, info};
            if (!componentsData.contains(component))
            {
                componentsData.append(component);
            }
        }
    }

    ComponentMimeData* mimeData = new ComponentMimeData(componentsData);
    for (const auto &format : data->formats())
    {
        mimeData->setData(format, data->data(format));
    }

    delete data;

    return mimeData;
}

QStringList LocalFileSystemModel::getBundlesInDir(const QDir &dir) const
{
    QStringList bundles;
    bundles = dir.entryList(QStringList() << "*.bndl" << "*.BNDL", QDir::Files);
    for (auto &bundle : bundles)
    {
        bundle = dir.absolutePath() + "/" + bundle;
    }
    return bundles;
}
