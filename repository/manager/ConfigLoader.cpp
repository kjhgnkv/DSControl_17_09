#include "ConfigLoader.hpp"
#include <QDebug>
#include <QDirIterator>
#include <QDomDocument>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QTextStream>
#include <tuple>

namespace dsrepository
{
ConfigLoader::ConfigLoader()
: components_ {}
, componentsToRemove_ {}
, root_ {}
{
}

std::map<QString, Component> ConfigLoader::update()
{
    QStringList items;

    QDir dir {root_};

    QDirIterator it(dir.path(), QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    componentsToRemove_.clear();
    while (it.hasNext())
    {
        auto path = it.next();

        if (auto found = components_.find(path); found == components_.end())
        {
            QFile file {path};
            if (file.open(QIODevice::ReadOnly))
            {
                QDomDocument xmlDoc;
                xmlDoc.setContent(&file);
                if (auto component = translate(xmlDoc.documentElement()); !component.type_.isEmpty())
                {
                    components_.insert({path, std::move(component)});
                }
                file.close();
            }
        }
        items.push_back(path);
    }

    for (auto it = components_.begin(); it != components_.end();)
    {
        if (!items.contains(it->first))
        {
            componentsToRemove_.insert({it->first, it->second});
            it = components_.erase(it);
        }
        else
        {
            it++;
        }
    }

    return components_;
}

std::map<QString, Component> ConfigLoader::load(const QDir &dir, const QDir &description)
{
    if (dir.isEmpty() || !dir.exists() || description.isEmpty() || !description.exists())
    {
        return {};
    }

    components_.clear();

    root_ = dir.path();

    QDirIterator it(dir.path(), QStringList() << "*.bndl", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        auto path = it.next();
        auto bundlePath = path;
        path.replace(dir.path(), description.path());
        path.replace("bndl", "xml");
        QFile file {path};
        if (file.open(QIODevice::ReadOnly))
        {
            QDomDocument xmlDoc;
            xmlDoc.setContent(&file);
            if (auto component = translate(xmlDoc.documentElement()); !component.type_.isEmpty())
            {
                //qDebug() << "BUNDLE PATH:" << bundlePath;
                components_.insert({bundlePath, component});
            }
            file.close();
        }
    }

    return components_;
}

std::map<QString, Component> ConfigLoader::extraLoad(const QDir &dir)
{
    if (!dir.exists() || dir.isEmpty())
    {
        return {};
    }

    std::map<QString, Component> retval;

    QDirIterator it(dir.path(), QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext())
    {
        auto path = it.next();
        QFile file {path};
        if (file.open(QIODevice::ReadOnly))
        {
            QDomDocument xmlDoc;
            xmlDoc.setContent(&file);
            if (auto component = translate(xmlDoc.documentElement()); !component.type_.isEmpty())
            {
                retval.insert({path, component});
            }
            file.close();
        }
    }

    return retval;
}

std::map<QString, Component> ConfigLoader::components() const
{
    return components_;
}

std::map<QString, Component> ConfigLoader::componentsToRemove() const
{
    return componentsToRemove_;
}

QString ConfigLoader::root() const
{
    return root_;
}

Component ConfigLoader::translate(const QDomNode &root)
{
    Component retval;

    if (!root.isNull())
    {
        if (root.nodeName() == "description")
        {
            retval.type_ = "ComponentNode";

            for (auto element = root.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
            {
                if (element.nodeName() == "BasicInfo")
                {
                    auto name = element.attributeNode("name").value();
                    auto system = element.attributeNode("system").value();
                    auto bit = element.attributeNode("bit").value();
                    auto title = element.attributeNode("name").value();
                    auto type = element.attributeNode("type").value();
                    auto category = element.attributeNode("name").value();

                    retval.caption_ = name;
                    retval.category_ = category;
                    retval.info_["caption"] = name;
                    retval.info_["componentType"] = name;
                    retval.info_["category"] = category;
                    retval.info_["brief"] = "Title: " + title + "\n" + "Type: " + type + "\n" + "Category: " + category;
                    retval.info_["moduleName"] = name;
                    retval.info_["moduleType"] = type;

                    //TODO parameters section?
                    QJsonObject componentInfo;

                    componentInfo["GUID"] = element.attributeNode("name").value();

                    componentInfo["Version"] = element.attributeNode("version").value();

                    retval.version_ = element.attributeNode("version").value();

                    componentInfo["Extension"] = element.attributeNode("name").value();

                    componentInfo["CreatedDate"] = element.attributeNode("name").value();

                    componentInfo["UpdatedDate"] = element.attributeNode("name").value();

                    componentInfo["UploadDate"] = element.attributeNode("name").value();

                    componentInfo["LastmodifiedDate"] = element.attributeNode("name").value();

                    componentInfo["Description"] = element.attributeNode("name").value();

                    retval.info_["componentInfo"] = componentInfo;

                }
                //TODO for test, remove
                QJsonArray in;
                QJsonArray out;
                QJsonObject obj = {
                    {  "id"      , 0}
                    , {"name"    , ""}
                    , {"protocol", ""}
                };
                in.push_back(obj);
                out.push_back(obj);
                retval.info_["messagesIn"] = in;
                retval.info_["messagesOut"] = out;
            }
        }
    }

    return retval;
}

Component ConfigLoader::translateOld(const QDomNode &root)
{
    Component retval;
    if (!root.isNull())
    {
        if (root.nodeName() == "ModuleClass")
        {
            retval.type_ = "ComponentNode";
            if (auto rootElement = root.toElement(); !rootElement.isNull())
            {
                auto name = rootElement.attributeNode("name").value();
                retval.caption_ = name;
                retval.info_["caption"] = name;
                retval.info_["componentType"] = name;
                auto title = rootElement.attributeNode("title").value();
                auto type = rootElement.attributeNode("type").value();
                auto category = rootElement.attributeNode("category").value();

                retval.category_ = category;
                retval.info_["category"] = category;

                retval.info_["brief"] = "Title: " + title + "\n" + "Type: " + type + "\n" + "Category: " + category;

                retval.info_["moduleName"] = name;
                retval.info_["moduleType"] = type;
            }

            // FIXME rewrite to json array
            QStringList messagesListIn;
            QStringList messagesListOut;
            QJsonObject componentInfo;
            QJsonArray in;
            QJsonArray out;
            for (auto element = root.firstChildElement(); !element.isNull(); element = element.nextSiblingElement())
            {
                QJsonObject obj;

                auto name = element.nodeName();
                if (name == "Send")
                {
                    auto messageId = element.attributeNode("id").value().toInt();
                    auto messageName = element.attributeNode("name").value();
                    auto messageProto = element.attributeNode("protocol").value();
                    messagesListIn.push_back(messageId + ' ' + messageName + ' ' + messageProto);
                    obj = {
                    {  "id"      , messageId}
                    , {"name"    , messageName}
                    , {"protocol", messageProto}
                    };
                    in.push_back(obj);
                }
                else if (name == "Recv")
                {
                    auto messageId = element.attributeNode("id").value().toInt();
                    auto messageName = element.attributeNode("name").value();
                    auto messageProto = element.attributeNode("protocol").value();
                    messagesListOut.push_back(messageId + ' ' + messageName + ' ' + messageProto);
                    obj = {
                    {  "id"      , messageId}
                    , {"name"    , messageName}
                    , {"protocol", messageProto}
                    };
                    out.push_back(obj);
                }
                else if (name == "GUID")
                {
                    componentInfo["GUID"] = element.attributeNode("name").value();
                }
                else if (name == "Version")
                {
                    componentInfo["Version"] = element.attributeNode("name").value();
                    // TODO
                    retval.version_ = element.attributeNode("name").value();
                }
                else if (name == "Extension")
                {
                    componentInfo["Extension"] = element.attributeNode("name").value();
                }
                else if (name == "CreatedDate")
                {
                    componentInfo["CreatedDate"] = element.attributeNode("name").value();
                }
                else if (name == "UpdatedDate")
                {
                    componentInfo["UpdatedDate"] = element.attributeNode("name").value();
                }
                else if (name == "UploadDate")
                {
                    componentInfo["UploadDate"] = element.attributeNode("name").value();
                }
                else if (name == "LastmodifiedDate")
                {
                    componentInfo["LastmodifiedDate"] = element.attributeNode("name").value();
                }
                else if (name == "Description")
                {
                    componentInfo["Description"] = element.attributeNode("name").value();
                }
            }
            retval.info_["componentInfo"] = componentInfo;
            retval.info_["messagesIn"] = in;
            retval.info_["messagesOut"] = out;
        }
    }

    return retval;
}

} // namespace dsrepository
