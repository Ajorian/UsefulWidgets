#pragma once

#include "xTreeWidget.h"

struct Panel : public NodeData
{
	QString name, title;
	Panel(const GXP::AtributesVector& fields);
	QString toString()const;
};