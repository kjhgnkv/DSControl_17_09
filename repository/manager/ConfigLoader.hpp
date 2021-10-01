// ConfigLoader.hpp

#pragma once

#include <QDir>
#include <QJsonObject>
#include <tuple>

class QDomNode;

namespace dsrepository
{
class Component
{
public:
    Component()
    {
    }

    Component(const QString &type, const QString &category, const QString &caption, const QString &version,
              const QJsonObject &info)
    : type_ {type}
    , category_ {category}
    , version_ {version}
    , caption_ {caption}
    , info_ {info}
    {
    }

    QString type_;
    QString category_;
    QString version_;
    QString caption_;
    QJsonObject info_;
};
/*!
\brief load config information from xml files and load this in MimeStoreModel
 */
class ConfigLoader
{
public:
    enum Config
    {
        Type = 0,
        Category,
        Info
    };
    /*!
    \param dir directory from which will be loaded xml files
    \param model MimeStoreModel in which will be load deserialize datas
     */
    ConfigLoader();

    std::map<QString, Component> update();

    std::map<QString, Component> load(const QDir &dir, const QDir &description);
    static std::map<QString, Component> extraLoad(const QDir &dir);

    std::map<QString, Component> components() const;

    std::map<QString, Component> componentsToRemove() const;

    QString root() const;

private:
    /*!
    \param root root node of xml file with description of component
    \brief translate DomNode to QJsonObject, which be used by mime for
      creating node on scene
    \return void QString and QJsonObject if root was not valid
     */
    static Component translate(const QDomNode &root);
    static Component translateOld(const QDomNode &root);

    std::map<QString, Component> components_;
    std::map<QString, Component> componentsToRemove_;

    QString root_;
};
} // namespace dsrepository
