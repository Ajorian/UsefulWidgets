#ifndef TEST_H
#define TEST_H

#include <QMainWindow>
#include "xTreeWidget.h"

#define XML_NODE_TYPE_PANEL "panel"
#define FIELD_NAME	"name"
#define FIELD_TITLE "title"

struct Panel : public NodeData
{
    QString name, title;
    Panel(const GXP::AtributesVector& fields)
    {
        for (size_t i=0; i<fields.size(); i++)
            {
                if (QString(fields[i].first.c_str()).toLower() == FIELD_NAME)
                    name = fields[i].second.c_str();
                else if (QString(fields[i].first.c_str()).toLower() == FIELD_TITLE)
                    title = fields[i].second.c_str();
            }
    }

    QString toString()const{return QString("%1=\"%2\" %3=\"%4\"").arg(FIELD_NAME).arg(name).arg(FIELD_TITLE).arg(title);}
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
