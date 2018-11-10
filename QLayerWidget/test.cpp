#include "test.h"

test::test(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.lineEdit, SIGNAL(textChanged(QString)), ui.layerWidget, SLOT(filter(QString)));
	connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addItem()));
	connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(deleteItem()));
    initActions();
}

void test::initActions()
{
    QVector<QAction*> actions;
    QAction* bld = new QAction(QIcon(":/icons/bold.png"), tr("&Bold"), this);
    bld->setStatusTip(tr("Bold Text"));
    connect(bld, &QAction::triggered, this, [=]()
    {
        LayerInfo info = ui.layerWidget->getCurrentLayer();
        if (info.removable)
            ui.layerWidget->currentItem()->setTextColor(QColor(255, 0, 0));
    });

    actions.append(bld);
    ui.layerWidget->setMenuActions(actions);
}

void test::addItem()
{
	ItemData* itm = new ItemData;
	itm->start = rand();
	itm->end = itm->start + 10;
	ui.layerWidget->addLayer(LayerInfo(itm, "", itm->getName(), true));
}

void test::deleteItem()
{
	ui.layerWidget->deleteLayer();
}

