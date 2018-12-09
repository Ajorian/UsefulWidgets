#pragma once

#include <QTreeWidget>
#include "xml/gxp.h"

struct NodeData
{
    QString type;
    NodeData(const QString _type, const GXP::AtributesVector& fields):type(_type){}
    QString toXML()const{return QString();} // you don't have to push '<' 'type' and '>' in the beginning and the end of the return string;
};

struct NodeInfo
{
#define ItemIsHidden 0x1000
	quint16 stateFlags=0;
	NodeData* data=0;

	static quint16 parseFlags(const QString);

	NodeInfo(){}
	NodeInfo(const QString type, const GXP::AtributesVector& fields) { data = new NodeData(type, fields); }
	QString flagsToString()const;
};
Q_DECLARE_METATYPE(NodeInfo);

class xTreeWidget : public QTreeWidget
{
	Q_OBJECT

public:
	xTreeWidget(QWidget *parent = Q_NULLPTR);
	~xTreeWidget();
    void readXML(const QString path);
	void saveToXML(const QString path)const;
	void setMenuActions(QVector<QAction*> actions) { menuActions = actions; }
	void filter(const QString str, const QVector<int> columns);

private slots:
	void handleMenu(const QPoint &);

private:
	QString rootType;
	QVector<QAction*> menuActions;
	void XMLtoTreeWidget(const GXP::parser& prs);
};
