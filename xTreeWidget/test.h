#ifndef TEST_H
#define TEST_H

#include <QMainWindow>
#include "xTreeWidget.h"

struct TestNodeData1 : public NodeData
{
    QString name;
    virtual QString toString()const{return QString("name=\"%1\"").arg(name);}
};

struct TestNodeData2 : public NodeData
{
    virtual QString toString()const{return QString();}
};

namespace Ui {
class test;
}

class test : public QMainWindow
{
    Q_OBJECT

public:
    explicit test(QWidget *parent = 0);
    ~test();

private slots:
    void on_pushButton_clicked();
    void on_led_search_textChanged(const QString &arg1);

private:
    Ui::test *ui;
};

#endif // TEST_H
