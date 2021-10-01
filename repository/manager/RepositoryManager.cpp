#include "RepositoryManager.hpp"
#include "ConfigLoader.hpp"
#include "models/DataBaseModel.hpp"
#include "models/LocalFileSystemModel.hpp"

#include "repository/DataBaseController.hpp"
#include "repository/FileSystemController.hpp"

#include "manager/DSConfiguration.h"
// TODO
// fix lib JlCompress
// #include "3rdparty/quazip/quazip/JlCompress.h"

#include <QFileDialog>
#include <QItemSelectionModel>
#include <QSqlError>

#define DS_REPOSITORY_PATH "./"

using namespace dscontrol;

namespace dsrepository
{
RepositoryManager::RepositoryManager(QObject* parent)
: QObject(parent)

, topModel_ {ModelType::FileSystem}
, bottomModel_ {ModelType::FileSystem}
, bottomSelectionModel_ {nullptr}
, topSelectionModel_ {nullptr}
{
    loader_ = new ConfigLoader {};
    loader_->load({DSConfiguration::Instance().bundlesFolder()}
    , {DSConfiguration::Instance().descriptionFolder()});

    fileSystemModel_ = new LocalFileSystemModel {loader_->root(), loader_, this};
    fileSystemController_ = new FileSystemController {loader_->root(), this};

    remoteDataBaseController_ = new DataBaseController {
    DSConfiguration::Instance().componentsFolder().append("/remoteDB")
    };
    remoteDataBaseModel_ = new DataBaseModel(remoteDataBaseController_->database(), "Repository");
    remoteDataBaseModel_->setController(remoteDataBaseController_);

    bottomSelectionModel_ = new QItemSelectionModel(fileSystemModel_);
    topSelectionModel_ = new QItemSelectionModel(fileSystemModel_);

    connect(remoteDataBaseController_, &DataBaseController::dbUpdated, remoteDataBaseModel_,
            &DataBaseModel::needUpdate);
}

RepositoryManager::~RepositoryManager()
{
}

dsrepository::LocalFileSystemModel* dsrepository::RepositoryManager::fileSystemModel() const
{
    return fileSystemModel_;
}

QItemSelectionModel* RepositoryManager::topSelectionModel() const
{
    return topSelectionModel_;
}

QItemSelectionModel* RepositoryManager::bottomSelectionModel() const
{
    return bottomSelectionModel_;
}

FileSystemController* RepositoryManager::fileSystemController() const
{
    return fileSystemController_;
}

DataBaseModel* RepositoryManager::remoteDataBaseModel() const
{
    return remoteDataBaseModel_;
}

DataBaseController* RepositoryManager::remoteDataBaseController() const
{
    return remoteDataBaseController_;
}

ModelType RepositoryManager::type(QAbstractItemModel* model)
{
    if (!model)
    {
        return ModelType::None;
    }

    ModelType retval {ModelType::None};

    if (dynamic_cast<QFileSystemModel*>(model))
    {
        retval = ModelType::FileSystem;
    }
    else if (dynamic_cast<QSqlTableModel*>(model))
    {
        retval = ModelType::DataBase;
    }
    else
    {
        retval = ModelType::None;
    }

    return retval;
}

QAbstractItemModel* RepositoryManager::model(ModelType type) const
{
    switch (type)
    {
        case ModelType::FileSystem:
        {
            return fileSystemModel_;
        }
        case ModelType::DataBase:
        {
            return remoteDataBaseModel_;
        }
        case ModelType::None:
        {
            return nullptr;
        }
    }

    return nullptr;
}

void RepositoryManager::changeTopModel(const ModelType &newTop)
{
    topModel_ = newTop;
    topSelectionModel_->reset();
    topSelectionModel_->setModel(model(newTop));
}

void RepositoryManager::changeBottomModel(const ModelType &newBottom)
{
    bottomModel_ = newBottom;
    bottomSelectionModel_->reset();
    bottomSelectionModel_->setModel(model(newBottom));
}

ModelType RepositoryManager::topModel() const
{
    return topModel_;
}

ModelType RepositoryManager::bottomModel() const
{
    return bottomModel_;
}

ConfigLoader* RepositoryManager::loader() const
{
    return loader_;
}

QList<ComponentData> RepositoryManager::topFilePath()
{
    QList<ComponentData> retval {};
    auto indexes = topSelectionModel_->selectedRows();

    switch (topModel_)
    {
        case ModelType::DataBase:
        {
            for (const auto &index : indexes)
            {
                retval.append(
                remoteDataBaseModel_->data(index, DataBaseModel::Role::DisplayRowRole).value<ComponentData>());
            }
            break;
        }
        case ModelType::FileSystem:
        {
            for (const auto &index : indexes)
            {
                retval.append({fileSystemModel_->filePath(index), ""});
            }
            break;
        }
        case ModelType::None:
        {
            break;
        }
    }

    return retval;
}

QList<ComponentData> RepositoryManager::botFilePath()
{
    QList<ComponentData> retval {};
    auto indexes = bottomSelectionModel_->selectedRows();

    switch (bottomModel_)
    {
        case ModelType::DataBase:
        {
            for (const auto &index : indexes)
            {
                retval.append(
                remoteDataBaseModel_->data(index, DataBaseModel::Role::DisplayRowRole).value<ComponentData>());
            }
            break;
        }
        case ModelType::FileSystem:
        {
            for (const auto &index : indexes)
            {
                retval.append({fileSystemModel_->filePath(index), ""});
            }
            break;
        }
        case ModelType::None:
        {
            break;
        }
    }

    return retval;
}

void RepositoryManager::addNewComponent(ModelType type)
{
    auto pathToFolder = QFileDialog::getExistingDirectory(nullptr, tr("Choose component folder"), "",
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    switch (type)
    {
        case ModelType::FileSystem:
        {
            if (!fileSystemController_->add(pathToFolder, loader_->root()))
            {
                qWarning() << fileSystemController_->lastError();
                return;
            }

            loadNewComponents();
            break;
        }
        case ModelType::DataBase:
        {
            auto components = ConfigLoader::extraLoad(QDir {pathToFolder});

            for (const auto &[path, component] : components)
            {
                // auto ret = JlCompress::compressDir("tmp.zip", QFileInfo{path}.dir().path(), true);

                auto bytes = tools::fileBytes("tmp.zip");
                DataBaseQuery query = remoteDataBaseController_->makeQuery(component.caption_, component.version_,
                                                                           bytes);
                remoteDataBaseController_->add(query);
            }
            break;
        }
        case ModelType::None:
        {
            break;
        }
    }
}

void RepositoryManager::removeComponent(ModelType type)
{
    //TODO remove for test removing
    return;

    QModelIndexList values {};
    QList<ComponentData> data {};

    if (type == topModel())
    {
        data = topFilePath();
    }
    else if (type == bottomModel())
    {
        data = botFilePath();
    }

    switch (type)
    {
        case ModelType::FileSystem:
        {
            for (const auto &c : data)
            {
                if (!fileSystemController_->remove(c.path_))
                {
                    qWarning() << fileSystemController_->lastError();
                    return;
                }
            }
            break;
        }
        case ModelType::DataBase:
        {
            for (const auto &component : data)
            {
                auto query = remoteDataBaseController_->makeQuery(component.name_, component.version_, component.data_);
                if (!remoteDataBaseController_->remove(query))
                {
                    qWarning() << fileSystemController_->lastError();
                    return;
                }
            }
            break;
        }
        case ModelType::None:
        {
            break;
        }
    }

    loadNewComponents();
}

void RepositoryManager::compareByBytes()
{
    QList<QByteArray> data {};

    auto topPathes = topFilePath();
    for (const auto &path : topPathes)
    {
        data.append(fileSystemController_->itemBytes(path.name_));
    }

    auto botPathes = botFilePath();
    for (const auto &path : botPathes)
    {
        data.append(fileSystemController_->itemBytes(path.name_));
    }

    bool res = false;

    //TODO: test; refactoring because of: warning C4996: 'QList <QByteArray>::toSet': Use QSet<T>(list.begin(), list.end()) instead.
    auto stringSet = QSet<QByteArray>(data.begin(), data.end());//data.toSet();

    if (stringSet.count() == 1 && data.count() > 1)
    {
        res = true;
    }

    emit bytesCompare(res, {topPathes + botPathes});
}

void RepositoryManager::loadNewComponents()
{
    loader_->update();
    emit contentHasChanged();
}

} // namespace dsrepository
