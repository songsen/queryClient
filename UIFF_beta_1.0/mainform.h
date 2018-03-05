#ifndef MAINFORM_H
#define MAINFORM_H

#include <QSplitter>
#include "sqltablemodel.h"
#include "form.h"
#include "tablewidget.h"
#include "plotter.h"


class mainForm : public QTabWidget
{
    Q_OBJECT
public:
    mainForm(QWidget *parent = 0);
    ~mainForm();
public slots:
    void comboxIndexChanged(int);
    void findFromSql();
    void fillTextEditFromTable();
    void setPlotter();
private:

    Form *rightForm;
    tablewidget *lefttablewidget;
    SqlTableModel *model;
    int date;
    QSplitter *splitter;
    trailerlistWidget *trailer;
        Plotter *plotter;
};

#endif // MAINFORM_H
