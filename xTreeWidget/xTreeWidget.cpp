#include "xTreeWidget.h"
#include <functional>
#include <QVariant>
#include <QTreeWidgetItem>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QMenu>

std::function<void(const GXP::NodeChildren &  children, QTreeWidgetItem* parent)> _parseChildren;
std::function<bool(QTreeWidgetItem* itm,bool parentMatched)> _filtItm;
std::function<QString(QTreeWidgetItem* itm, int level)> _toXML;

#define ATRIB_ICON "icon"
#define ATRIB_TREE_STATE "treeState"
#define TREE_STATE_HIDE "hidden"
#define TREE_STATE_DISABLE "disable"
#define TREE_STATE_SELECTABLE "selectable"
#define TREE_STATE_EDITABLE "editable"
#define TREE_STATE_CHECKABLE "checkable"
#define FLAGS_SEPRATOR	','
#define GET_DATA(itm_ptr) ((itm_ptr)->data(0,Qt::UserRole).value<NodeInfo>())

quint16 NodeInfo::parseFlags(const QString val)
{
	quint16 flags = Qt::ItemIsEnabled;
	const QStringList sts = val.split(FLAGS_SEPRATOR);
	for (int t = 0; t < sts.size(); t++)
	{
		if (sts[t] == TREE_STATE_HIDE) flags |= ItemIsHidden;
		else if (sts[t] == TREE_STATE_DISABLE) flags &= ~((quint16)Qt::ItemIsEnabled);
		else if (sts[t] == TREE_STATE_SELECTABLE) flags |= Qt::ItemIsSelectable;
		else if (sts[t] == TREE_STATE_EDITABLE) flags |= Qt::ItemIsEditable;
		else if (sts[t] == TREE_STATE_CHECKABLE) flags |= Qt::ItemIsUserCheckable;
	}
	return flags;
}

QString NodeInfo::flagsToString()const
{
	QString ret;
	if (stateFlags & ItemIsHidden) ret += TREE_STATE_HIDE;
	else if (stateFlags & Qt::ItemIsSelectable) { if (!ret.isEmpty())ret += QString(FLAGS_SEPRATOR); ret+=TREE_STATE_SELECTABLE;}
	else if (stateFlags & Qt::ItemIsEditable)   { if (!ret.isEmpty())ret += QString(FLAGS_SEPRATOR); ret += TREE_STATE_EDITABLE;}
	else if (stateFlags & Qt::ItemIsUserCheckable) { if (!ret.isEmpty())ret += QString(FLAGS_SEPRATOR); ret += TREE_STATE_CHECKABLE; }
	else if (!(stateFlags & Qt::ItemIsEnabled)) { if (!ret.isEmpty())ret += QString(FLAGS_SEPRATOR); ret += TREE_STATE_DISABLE; }
	return ret;
}

xTreeWidget::xTreeWidget(QWidget *parent)
	: QTreeWidget(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &QTreeWidget::customContextMenuRequested, this, &xTreeWidget::handleMenu);
}

xTreeWidget::~xTreeWidget()
{
}

void xTreeWidget::readXML(const QString path)
{
    clear();
	const GXP::parser pars(path.toStdString().c_str());
	rootType = pars.data->type.c_str();
	XMLtoTreeWidget(pars);
}

void xTreeWidget::saveToXML(const QString path) const
{
	QFile file(path);
	if (file.open(QIODevice::ReadWrite))
	{
		QTextStream stream(&file);
		stream << "<?xml version=\"1.0\" encoding=\"WINDOWS-1251\" ?>\n";
		stream << QString("<%1>").arg(rootType)<<endl;

		_toXML = [&](QTreeWidgetItem* itm, int level)
		{
			const NodeInfo nInfo = GET_DATA(itm);
			const QString xmlStr = nInfo.data->toXML();
			QString ret;

			if (!xmlStr.isEmpty())
			{
				for (int i = 0; i < level; i++) ret +="\t";
				const QString flgs = nInfo.flagsToString();
				const QString treeStateStr = flgs.isEmpty()? QString() : QString("%1=\"%2\"").arg(ATRIB_TREE_STATE).arg(flgs);
				ret += QString ("<%1 %2 ").arg(nInfo.data->type).arg(treeStateStr)+xmlStr+((itm->childCount()>0)?" >":" />")+"\n";
				for (int i = 0; i < itm->childCount(); i++)
					ret += _toXML(itm->child(i), level+1);
				if (itm->childCount() > 0)
				{
					for (int i = 0; i < level; i++) ret += "\t";
					ret += QString("</%1>\n").arg(nInfo.data->type);
				}
			}
			return ret;
		};

		for (int i = 0; i < topLevelItemCount(); i++)
		{
			const QString xmlStr = _toXML(topLevelItem(i),1);
			if(!xmlStr.isEmpty())stream << xmlStr;
		}
 		stream << QString("</%1>").arg(rootType);
	}
}

void xTreeWidget::XMLtoTreeWidget(const GXP::parser& pars)
{
	QStringList header;
	QList<bool> hiddenColumns;

    auto _createTreeItem = [&](const GXP::AtributesVector &  atribs,quint16 & flags)
 	{
		QStringList row;QString iconPath;QList<int> colIdxs;
		flags = Qt::ItemIsEnabled;

        for (size_t j = 0; j < atribs.size(); j++)
		{
			const QString field = atribs[j].first.c_str();
			if (field == ATRIB_TREE_STATE)
                flags = NodeInfo::parseFlags(QString(atribs[j].second.c_str()));
			else if (field == ATRIB_ICON)
			{
				if(!atribs[j].second.empty())
					iconPath = QApplication::applicationDirPath() + "/icons/" + atribs[j].second.c_str();
			}
			else
			{
				if (!header.contains(field)) { header << field; hiddenColumns << true; }
				const int idx = header.indexOf(field);
				colIdxs << idx;
				for (int k = row.size(); k <= idx; k++)	row << "";
				row[idx] = atribs[j].second.c_str();
			}
		}

		if (!(flags&ItemIsHidden)) { for (int i = 0; i < colIdxs.size(); i++) hiddenColumns[colIdxs[i]] = false; }
		QTreeWidgetItem* itm = new QTreeWidgetItem(row);
		if (flags & Qt::ItemIsUserCheckable) itm->setCheckState(0,Qt::Unchecked);
		itm->setFlags((Qt::ItemFlags)flags);
		if(!iconPath.isEmpty())
			itm->setIcon(0, QIcon(iconPath));
		return itm;
 	};

    _parseChildren = [&](const GXP::NodeChildren &  children, QTreeWidgetItem* parent)
	{
		quint16 state = 0;
        for (size_t i = 0; i < children.size(); i++)
		{
			const GXP::AtributesVector & atribs = children[i]->atributes;
            QTreeWidgetItem* treeItm = _createTreeItem(atribs,state);
			NodeInfo info(children[i]->type.c_str(), atribs);
			info.stateFlags = state;
			treeItm->setData(0,Qt::UserRole, QVariant::fromValue<NodeInfo>(info));

            if (parent == NULL) addTopLevelItem(treeItm);
			else parent->addChild(treeItm);
			treeItm->setHidden(state&ItemIsHidden);
            _parseChildren(children[i]->children, treeItm);
			treeItm->setExpanded(true);
		}
	};

    _parseChildren(pars.data->children,NULL);
    setColumnCount(header.count());
    setHeaderLabels(header);
	for (int i = 0; i < hiddenColumns.size(); i++)
        setColumnHidden(i, hiddenColumns[i]);
}

void xTreeWidget::filter(const QString str,const QVector<int> columns)
{
    _filtItm=[&](QTreeWidgetItem* itm,bool parentMatched)
    {
        bool match = parentMatched;
        if(!match)
        {
            for(int i=0; i<columns.size(); i++)
            {
                if(itm->text(columns[i]).toLower().contains(str.toLower()))
                {
                    match=true;
                    break;
                }
            }
        }
		bool childrenMatched = false;
        for(int i=0; i<itm->childCount(); i++)
			childrenMatched |= _filtItm(itm->child(i),match);
		match |= childrenMatched;

        const bool hidden = ((GET_DATA(itm).stateFlags) & ItemIsHidden) || !match;
        itm->setHidden(hidden);
        return match;
    };

    for(int i=0; i<topLevelItemCount(); i++)
        _filtItm(topLevelItem(i),false);
}

void xTreeWidget::handleMenu(const QPoint & pos)
{
	if (menuActions.isEmpty())
		return;
	
	QMenu menu(0);
	for (int i = 0; i < menuActions.size(); i++)
		menu.addAction(menuActions[i]);
	menu.exec(mapToGlobal(pos));
}
