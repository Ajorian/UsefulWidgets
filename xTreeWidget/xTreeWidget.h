#pragma once

#include <QWidget>
#include "ui_xTreeWidget.h"
#include "xml/gxp.h"

struct NodeData
{
	NodeData(const QString type, const GXP::AtributesVector& fields){}
};

struct NodeInfo
{
#define ItemIsHidden 0x1000
	quint16 stateFlags=0;
	NodeData* data=0;

	NodeInfo(){}
	NodeInfo(const QString type, const GXP::AtributesVector& fields) { data = new NodeData(type, fields); }
};
Q_DECLARE_METATYPE(NodeInfo);

class xTreeWidget : public QWidget
{
	Q_OBJECT

public:
	xTreeWidget(QWidget *parent = Q_NULLPTR);
	~xTreeWidget();
    void readXML(const QString path);
    void clear(){ui.tree->clear();}

private slots:
    void on_led_search_textChanged(const QString &arg1);

private:
    Ui::xTreeWidget ui;
	void XMLtoTreeWidget(const GXP::parser& prs);
    void filter(const QString str,const QVector<int> columns);
};
