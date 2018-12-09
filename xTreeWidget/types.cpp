#include "types.h"

#define XML_NODE_TYPE_PANEL "panel"
#define FIELD_NAME	"name"
#define FIELD_TITLE "title"

NodeData* CreateNodeData(const QString type, const GXP::AtributesVector& fields)
{
	if (type.toLower() == XML_NODE_TYPE_PANEL)
		return new Panel(fields);
	return new EmptyNodeData;
}

Panel::Panel(const GXP::AtributesVector& fields)
{
    for (size_t i=0; i<fields.size(); i++)
	{
		if (QString(fields[i].first.c_str()).toLower() == FIELD_NAME)
			name = fields[i].second.c_str();
		else if (QString(fields[i].first.c_str()).toLower() == FIELD_TITLE)
			title = fields[i].second.c_str();
	}
}

QString Panel::toString() const
{
	return QString("%1=\"%2\" %3=\"%4\"").arg(FIELD_NAME).arg(name).arg(FIELD_TITLE).arg(title);
}
