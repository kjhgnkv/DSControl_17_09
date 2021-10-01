#pragma once

#include <QObject>

namespace dsrepository
{
namespace repository
{
class AbstractController : public QObject
{
public:
    explicit AbstractController(QObject* parent = nullptr);
    virtual ~AbstractController() override;

    virtual bool add(const QString &source, const QString &dest, bool allowReplacement = false) = 0;
    virtual bool copy(const QString &source, const QString &dest, bool allowReplacement = false) = 0;
    virtual bool move(const QString &source, const QString &dest, bool allowReplacement = false) = 0;
    virtual bool remove(const QString &source) = 0;
    virtual QByteArray itemBytes(const QString &path) = 0;

    QString lastError() const;

protected:
    QString lastError_;
};
} // namespace repository
} // namespace dsrepository
