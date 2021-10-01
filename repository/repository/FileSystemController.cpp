#include "FileSystemController.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>

namespace dsrepository
{
namespace repository
{
FileSystemController::FileSystemController(QString root, QObject* parent)
: AbstractController(parent)
, root_ {root}
{
}

FileSystemController::~FileSystemController()
{
}

bool FileSystemController::add(const QString &source, const QString &dest, bool allowReplacement)
{
    QString modSource = source;
    QString modDest = dest;

    return copy(modSource, modDest, allowReplacement);
}

bool FileSystemController::copy(const QString &source, const QString &dest, bool allowReplacement)
{
    bool retval {false};
    QString modSource = source;
    QString modDest = dest;

    validate(modSource, modDest);

    QDir sourceDir {modSource};
    QDir destDir {modDest};

    retval = copyRecursively(sourceDir, destDir, allowReplacement);

    if (!retval)
    {
        lastError_ = "Failed to copy";
    }

    return retval;
}

bool FileSystemController::move(const QString &source, const QString &dest, bool allowReplacement)
{
    bool retval {false};

    QString modSource = source;
    QString modDest = dest;

    validate(modSource, modDest);

    QDir dir;

    if (dir.exists(dest) && allowReplacement)
    {
        dir.setPath(modDest);
        dir.removeRecursively();
    }

    retval = dir.rename(modSource, modDest);

    if (!retval)
    {
        lastError_ = "Failed to move";
    }

    return retval;
}

bool FileSystemController::remove(const QString &source)
{
    bool retval {false};

    QString modSource = source;
    QString tmp {};

    validate(modSource, tmp);
    QDir dir {modSource};
    retval = dir.removeRecursively();

    if (!retval)
    {
        lastError_ = "Failed to remove!";
    }

    return retval;
}

bool FileSystemController::copyRecursively(QDir sourceDir, QDir destDir, bool allowReplacement)
{
    bool success = false;

    auto sourceFolder = sourceDir.path();
    auto destFolder = destDir.path();

    if (!sourceDir.exists())
    {
        return false;
    }

    if (destDir.exists() && allowReplacement)
    {
        destDir.removeRecursively();
    }
    else if (!destDir.exists())
    {
        destDir.mkpath(".");
    }

    QStringList files = sourceDir.entryList(QDir::Files);
    for (int i = 0; i < files.count(); i++)
    {
        QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
        success = QFile::copy(srcName, destName);
        if (!success)
        {
            return false;
        }
    }

    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for (int i = 0; i < files.count(); i++)
    {
        QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
        success = copyRecursively(srcName, destName);
        if (!success)
            return false;
    }

    return true;
}

std::pair<QString, QString> FileSystemController::validate(QString &source, QString &dest)
{
    QFileInfo info {source};
    QString dirName {};
    if (!info.isDir())
    {
        source = info.absoluteDir().path();
        dirName = info.dir().dirName();
    }
    else
    {
        dirName = info.fileName();
    }

    if (QDir dir {dest}; dir.exists())
    {
        dest += QDir::separator() + dirName;
    }

    return {source, dest};
}

QByteArray FileSystemController::itemBytes(const QString &path)
{
    QByteArray retval;

    QFileInfo info {path};

    QFile file {path};
    if (!file.open(QIODevice::ReadOnly) || info.isDir())
    {
        lastError_ = file.errorString();
        qDebug() << lastError_;
        return {};
    }

    retval = file.readAll();

    return retval;
}
} // namespace repository
} // namespace dsrepository
