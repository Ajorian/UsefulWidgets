#include "test.h"
#include "ui_test.h"
#include <QFileDialog>

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
        ui->xtreeWid->clear();
        ui->led_path->setText(strPath);
        ui->xtreeWid->readXML(strPath);
    }
}
