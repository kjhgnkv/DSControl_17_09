#pragma once

#include "repository_globals.hpp"
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QString>

namespace dsrepository
{
namespace tools
{
// TODO Path FIX
// сюда передается путь и имя dll|so
// но, где долно вызыватся?
static QString folderToComponentPath(const QString &path, const QString &componentName)
{
    QString retval {};

    QFileInfo info {path};

    QString suffix {};
    #ifdef WIN32
    suffix = ".dll";
    #elif __linux
    suffix = ".so";
    #endif
    retval = path + "/" + "lib" + componentName + suffix;

    return retval;
}

static QString componentToConfig(const QString &path)
{
    QString retval {};

    QFileInfo info {path};
    auto dir = info.dir();
    info.setFile(dir.path());
    auto name = info.fileName();
    retval = path + "/" + name + ".xml";
    qDebug() << "componentToConfig";
    qDebug() << "name: " + name;
    qDebug() << "retval: " + retval;

    return retval;
}

static QString componentName(const QString &path)
{

    QFileInfo info {path};
    auto dir = info.dir();
    info.setFile(dir.path());

    return info.fileName();
}

static QString componentToFolder(QString name)
{
    QFileInfo info {name};

    return info.path();
}

static QString createFilePathToComponentExtraction(const QString &parent, const ComponentData &component)
{
    QDir dir {parent};
    QString retval;

    if (dir.dirName() == component.name_)
    {
        retval = "V" + component.version_;
        dir.mkdir(retval);
    }
    else
    {
        retval = component.name_ + "/V" + component.version_;
        dir.mkpath(retval);
    }

    retval.push_front(dir.path());

    return retval;
}

static QByteArray fileBytes(const QString &path)
{
    QByteArray retval {};

    QFile file {path};

    if (!file.open(QIODevice::ReadOnly))
    {
        return retval;
    }

    retval = file.readAll();
    file.close();

    return retval;
}

static void createFile(const QString &path, const QByteArray &data)
{
    QFile file {path};
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.waitForBytesWritten(10000);
    file.close();
}

} // namespace tools
} // namespace dsrepository
