#include "ComponentMimeData.hpp"

using namespace dsrepository;

ComponentMimeData::ComponentMimeData(QList<dsrepository::ComponentData> components)
: components_ {components}
{
}

QString ComponentMimeData::mimeType()
{
    return "application/ComponentMimeData";
}

QList<ComponentData> ComponentMimeData::components() const
{
    return components_;
}

bool ComponentMimeData::hasFormat(const QString &mimetype) const
{
    if (mimetype == mimeType())
    {
        return true;
    }

    return QMimeData::hasFormat(mimetype);
}

QStringList ComponentMimeData::formats() const
{
    auto list = QMimeData::formats();
    list.append(mimeType());
    return list;
}
