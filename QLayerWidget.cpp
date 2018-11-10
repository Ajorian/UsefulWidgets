#include "QLayerWidget.h"
#include <QMenu>

#define GET_DATA(itm_ptr) ((itm_ptr)->data(Qt::UserRole).value<LayerInfo>())

QLayerWidget::QLayerWidget(QWidget* parent) : QListWidget(parent)
{
	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(handleMenu(const QPoint&)));
    connect(this, &QLayerWidget::itemDoubleClicked, this, [=](QListWidgetItem* itm)
	{
		emit layerDoubleClicked(GET_DATA(itm));
	});
}

QLayerWidget::~QLayerWidget()
{
	freeLayers();
}

void QLayerWidget::freeLayers()
{
	for (quint16 i = 0; i < count(); i++)
		delete (item(i)->data(Qt::UserRole).value<void*>());
	clear();
}

void QLayerWidget::addLayer(const LayerInfo info)
{
	QListWidgetItem* itm = new QListWidgetItem(QIcon(info.icon),info.name);
	itm->setData(Qt::UserRole, QVariant::fromValue<LayerInfo>(info));
	addItem(itm);
	itm->setSelected(true);
	setCurrentItem(itm);
}

void QLayerWidget::deleteLayer()
{
	QListWidgetItem *it = takeItem(currentRow());
	if (!it) return;
	LayerInfo info = it->data(Qt::UserRole).value<LayerInfo>();
	delete info.data;
    delete it;
}

void QLayerWidget::filter(QString arg)
{
    for(int i=0; i<count(); i++)
    {
        QListWidgetItem* itm = item(i);
		QString s = itm->text();
        itm->setHidden(!itm->text().contains(arg));
    }
	setCurrentItem(NULL);
}

void QLayerWidget::loadLayers(const QVector<LayerInfo> layers)
{
	freeLayers();
	for (int i = 0; i < layers.size(); i++)
		addLayer(layers[i]);
}

LayerInfo QLayerWidget::getLayer(int idx)
{
	QListWidgetItem* currItm = item(idx);
	if (currItm)
		return GET_DATA(currItm);
	return LayerInfo();
}

LayerInfo QLayerWidget::getCurrentLayer()
{
	QListWidgetItem* itm = currentItem();
	if (itm)
		return GET_DATA(itm);
	return LayerInfo();
}

void QLayerWidget::handleMenu(const QPoint & pos)
{
	if (menuActions.isEmpty())
		return;

	QListWidgetItem* itm = itemAt(pos);
	if (!GET_DATA(itm).removable)
		return;

	QMenu menu(0);
	for (int i=0; i<menuActions.size(); i++)
		menu.addAction(menuActions[i]);
	menu.exec(mapToGlobal(pos));
}
