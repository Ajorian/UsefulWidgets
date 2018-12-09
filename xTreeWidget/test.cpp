#include "test.h"
#include "ui_test.h"
#include <QFileDialog>

NodeData* CreateNodeData(const QString type, const GXP::AtributesVector& fields)
{
    if(type=="test")
        return new TestNodeData1();
    return new TestNodeData2();
}

test::test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::test)
{
    ui->setupUi(this);
}

test::~test()
{
    delete ui;
}

void test::on_pushButton_clicked()
{
    QString strPath = ui->led_path->text();
    strPath = QFileDialog::getOpenFileName(this, "Select XML File", "", "XML Files (*.xml)");
    if (strPath.isEmpty())
        return;
    else
    {
        ui->tree->clear();
        ui->led_path->setText(strPath);
        ui->tree->readXML(strPath);
    }
}

void test::on_led_search_textChanged(const QString &arg1)
{
    ui->tree->filter(arg1,QVector<int>()<<0);
}
