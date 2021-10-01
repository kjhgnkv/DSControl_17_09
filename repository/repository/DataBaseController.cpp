#include "DataBaseController.hpp"
#include <QDebug>
#include <QFileInfo>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

using namespace dsrepository;
using namespace dsrepository::repository;

DataBaseController::DataBaseController(const QString &pathToDb, QObject* parent)
: QObject(parent)
{
    dbSettings_.name = pathToDb;
    init();
}

DataBaseController::~DataBaseController()
{
}

bool DataBaseController::add(const DataBaseQuery &query)
{
    if (!query.isValid())
    {
        return false;
    }

    QSqlQuery sqlQuery {db_};

    sqlQuery.prepare(
    QString("INSERT INTO %1 (%2, %3, %4) VALUES(:Name, :Version, :Data)").arg("Repository").arg("Name").arg(
    "Version").arg("Data"));
    sqlQuery.bindValue(":Name", query.name);
    sqlQuery.bindValue(":Version", query.version);
    sqlQuery.bindValue(":Data", query.data);

    if (!sqlQuery.exec())
    {
        qWarning() << "Cannot insert";
        qWarning() << sqlQuery.lastError();
        return false;
    }

    emit dbUpdated();

    return true;
}

bool DataBaseController::remove(const DataBaseQuery &query)
{
    if (!query.isValid())
    {
        return false;
    }

    QSqlQuery sqlQuery {db_};

    sqlQuery.prepare(
    QString("DELETE FROM %1 WHERE Name LIKE :Name AND Version LIKE :Version").arg(dbSettings_.tableName));
    sqlQuery.bindValue(":Name", query.name);
    sqlQuery.bindValue(":Version", query.version);

    if (!sqlQuery.exec())
    {
        qWarning() << "Cannot delete";
        qWarning() << sqlQuery.lastQuery();
        qWarning() << sqlQuery.lastError();
        return false;
    }

    emit dbUpdated();

    return true;
}

DataBaseQuery DataBaseController::makeQuery(const QString &name, const QString &version, const QByteArray &data)
{
    return {name, version, data};
}

QSqlDatabase &DataBaseController::database()
{
    return db_;
}

void DataBaseController::init()
{
    if (!open())
    {
        qWarning() << "Cannot initialize controller";
    }
}

bool DataBaseController::open()
{
    db_ = QSqlDatabase::addDatabase("QSQLITE");
    db_.setDatabaseName(dbSettings_.name);

    if (!db_.open())
    {
        qWarning() << "Cannot open database!";
        qWarning() << db_.lastError();
        return false;
    }

    return true;
}

void DataBaseController::close()
{
}
