#pragma once

#include "repository_globals.hpp"
#include <QObject>

class QAbstractItemModel;
class QItemSelectionModel;

namespace dsrepository
{
namespace models
{
class LocalFileSystemModel;
class DataBaseModel;
} // namespace models

namespace repository
{
class FileSystemController;
class DataBaseController;
} // namespace repository

class ConfigLoader;

using namespace models;
using namespace repository;

class RepositoryManager : public QObject
{
Q_OBJECT
public:
    explicit RepositoryManager(QObject* parent = nullptr);
    ~RepositoryManager() override;

    LocalFileSystemModel* fileSystemModel() const;
    FileSystemController* fileSystemController() const;

    DataBaseModel* remoteDataBaseModel() const;
    DataBaseController* remoteDataBaseController() const;

    QItemSelectionModel* topSelectionModel() const;
    QItemSelectionModel* bottomSelectionModel() const;

    ModelType type(QAbstractItemModel* model);
    QAbstractItemModel* model(ModelType type) const;

    void changeTopModel(const ModelType &newTop);
    void changeBottomModel(const ModelType &newBottom);

    ModelType topModel() const;
    ModelType bottomModel() const;

    ConfigLoader* loader() const;

    QList<ComponentData> topFilePath();
    QList<ComponentData> botFilePath();

public slots:
    void addNewComponent(ModelType type);
    void removeComponent(ModelType type);

    void compareByBytes();

private slots:
    void loadNewComponents();

signals:
    void bytesCompare(bool result, QList<ComponentData>);
    void contentHasChanged();

private:
    ModelType topModel_;
    ModelType bottomModel_;

    LocalFileSystemModel* fileSystemModel_;
    QItemSelectionModel* bottomSelectionModel_;
    QItemSelectionModel* topSelectionModel_;
    FileSystemController* fileSystemController_;

    DataBaseModel* remoteDataBaseModel_;
    DataBaseController* remoteDataBaseController_;

    ConfigLoader* loader_;
};

} // namespace dsrepository
