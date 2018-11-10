#pragma once

#include <QListWidget>

struct LayerInfo
{
	void* data = NULL;
	QString icon;
	QString name;
	bool removable=true;
	LayerInfo(void* _data, QString _icon, QString _name,bool _removable=true) : data(_data), icon(_icon), name(_name) ,removable(_removable){}
	LayerInfo() {}
};
Q_DECLARE_METATYPE(LayerInfo);

class QLayerWidget : public QListWidget
{
	Q_OBJECT

	void freeLayers();
	QVector<QAction*> menuActions;

public:
    QLayerWidget(QWidget* parent);
    ~QLayerWidget();
	void addLayer(const LayerInfo);
 	void loadLayers(const QVector<LayerInfo>);
	LayerInfo getLayer(int idx);
	LayerInfo getCurrentLayer();
	void setMenuActions(QVector<QAction*> actions) { menuActions = actions; }

public slots:
	void deleteLayer();
    void filter(QString);

private slots:
	void handleMenu(const QPoint & pos);

signals:
	void layerDoubleClicked(LayerInfo);
};
