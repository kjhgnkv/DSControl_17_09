#pragma once

#include <QObject>
#include <QSqlDatabase>

//
#include "AbstractController.hpp"
#include "DataBaseQuery.hpp"
#include "DataBaseSettings.hpp"

namespace dsrepository
{
namespace repository
{
class DataBaseController : public QObject
{
Q_OBJECT
public:
    explicit DataBaseController(const QString &pathToDb, QObject* parent = nullptr);
    ~DataBaseController() override;

    bool add(const DataBaseQuery &query);
    bool remove(const DataBaseQuery &query);

    DataBaseQuery makeQuery(const QString &name, const QString &version, const QByteArray &data = {});

    QSqlDatabase &database();

signals:
    void dbUpdated();

private:
    void init();
    bool open();
    void close();

private:
    QSqlDatabase db_;
    DataBaseSetting dbSettings_;
};

} // namespace repository
} // namespace dsrepository
