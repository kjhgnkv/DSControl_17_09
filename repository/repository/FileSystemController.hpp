#pragma once

#include <QDir>
#include <QObject>

#include "AbstractController.hpp"

namespace dsrepository
{
namespace repository
{
class FileSystemController : public AbstractController
{
Q_OBJECT
public:
    explicit FileSystemController(QString root, QObject* parent = nullptr);
    ~FileSystemController() override;

    bool add(const QString &source, const QString &dest, bool allowReplacement = false) override;
    bool copy(const QString &source, const QString &dest, bool allowReplacement = false) override;
    bool move(const QString &source, const QString &dest, bool allowReplacement = false) override;
    bool remove(const QString &source) override;
    QByteArray itemBytes(const QString &path) override;

private:
    bool copyRecursively(QDir sourceDir, QDir destDir, bool allowReplacement = false);

    std::pair<QString, QString> validate(QString &source, QString &dest);
    std::pair<QString, QString> validate(QString &source);

private:
    QString root_;
};

} // namespace repository
} // namespace dsrepository
