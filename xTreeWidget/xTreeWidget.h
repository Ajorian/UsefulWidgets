#pragma once

#include <QWidget>
#include "ui_xTreeWidget.h"
#include "xml/gxp.h"

struct NodeData
{
    QString type;
    NodeData(const QString _type, const GXP::AtributesVector& fields):type(_type){}
	QString toXML()const // you don't have to push '<' 'type' and '>' in the beginning and the end of the return string;
	{
        return QString();
	}
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

class xTreeWidget : public QWidget
{
	Q_OBJECT

public:
	xTreeWidget(QWidget *parent = Q_NULLPTR);
	~xTreeWidget();
	void clear() { ui.tree->clear(); }
    void readXML(const QString path);
	void saveToXML(const QString path)const;

private slots:
    void on_led_search_textChanged(const QString &arg1);

private:
    Ui::xTreeWidget ui;
	QString rootType;
	void XMLtoTreeWidget(const GXP::parser& prs);
    void filter(const QString str,const QVector<int> columns);
};
