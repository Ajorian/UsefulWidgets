#include "test.h"

test::test(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	connect(ui.lineEdit, SIGNAL(textChanged(QString)), ui.layerWidget, SLOT(filter(QString)));
	connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addItem()));
	connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(deleteItem()));
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
