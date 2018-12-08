#include "xTreeWidget.h"
#include <functional>
#include <QVariant>
#include <QTreeWidgetItem>
#include <QApplication>

std::function<void(const GXP::NodeChildren &  children, QTreeWidgetItem* parent)> _parseChildren;
std::function<bool(QTreeWidgetItem* itm,bool parentMatched)> _filtItm;

#define ATRIB_ICON "icon"
#define ATRIB_TREE_STATE "treeState"
#define TREE_STATE_HIDE "hidden"
#define TREE_STATE_DISABLE "disable"
#define TREE_STATE_SELECTABLE "selectable"
#define TREE_STATE_EDITABLE "editable"
#define TREE_STATE_CHECKABLE "checkable"
#define GET_DATA(itm_ptr) ((itm_ptr)->data(0,Qt::UserRole).value<NodeInfo>())

xTreeWidget::xTreeWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

xTreeWidget::~xTreeWidget()
{
}

void xTreeWidget::readXML(const QString path)
{
	const GXP::parser pars(path.toStdString().c_str());
	XMLtoTreeWidget(pars);
}

void xTreeWidget::XMLtoTreeWidget(const GXP::parser& pars)
{
	QStringList header;
	QList<bool> hiddenColumns;

    auto _setFlags= [](const QString val)
	{
		quint16 flags = Qt::ItemIsEnabled;
		const QStringList sts = val.split(',');
		for (int t = 0; t < sts.size(); t++)
		{
			if (sts[t] == TREE_STATE_HIDE) flags |= ItemIsHidden;
			else if (sts[t] == TREE_STATE_DISABLE) flags &= ~((quint16)Qt::ItemIsEnabled);
			else if (sts[t] == TREE_STATE_SELECTABLE) flags |= Qt::ItemIsSelectable;
			else if (sts[t] == TREE_STATE_EDITABLE) flags |= Qt::ItemIsEditable;
			else if (sts[t] == TREE_STATE_CHECKABLE) flags |= Qt::ItemIsUserCheckable;
		}
		return flags;
	};

    auto _createTreeItem = [&](const GXP::AtributesVector &  atribs,quint16 & flags)
 	{
		QStringList row;QString iconPath;QList<int> colIdxs;
		flags = Qt::ItemIsEnabled;

        for (size_t j = 0; j < atribs.size(); j++)
		{
			const QString field = atribs[j].first.c_str();
			if (field == ATRIB_TREE_STATE)
                flags = _setFlags(QString(atribs[j].second.c_str()));
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

            if (parent == NULL) ui.tree->addTopLevelItem(treeItm);
			else parent->addChild(treeItm);
			treeItm->setHidden(state&ItemIsHidden);
            _parseChildren(children[i]->children, treeItm);
			treeItm->setExpanded(true);
		}
	};

    _parseChildren(pars.data->children,NULL);
    ui.tree->setColumnCount(header.count());
    ui.tree->setHeaderLabels(header);
	for (int i = 0; i < hiddenColumns.size(); i++)
        ui.tree->setColumnHidden(i, hiddenColumns[i]);
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

    for(int i=0; i<ui.tree->topLevelItemCount(); i++)
        _filtItm(ui.tree->topLevelItem(i),false);
}

void xTreeWidget::on_led_search_textChanged(const QString &arg1)
{
    filter(arg1,QVector<int>()<<0);
}
