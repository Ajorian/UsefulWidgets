#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_test.h"

struct ItemData 
{
	int start = 0, end = 0;
	QString getName()const { return QString("%1-%2").arg(QString::number(start)).arg(QString::number(end)); }
};

class test : public QMainWindow
{
    Q_OBJECT

public:
    test(QWidget *parent = Q_NULLPTR);

private:
    Ui::tstClass ui;  
    void initActions();

private slots:
	void addItem();
	void deleteItem();
};
