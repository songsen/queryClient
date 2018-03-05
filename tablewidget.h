#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include "sqltablemodel.h"


class tablewidget : public QTableView
{
    Q_OBJECT

public:
    tablewidget(QWidget *parent=0);
    ~tablewidget();
inline  void setTableFrom();

    //void addRow();

public slots:
    void showToolTip(const QModelIndex &index);
    void findTable( );
    void comoboxTable(int);
    void tableClicked(QModelIndex);
    void sqlModelAdd();
    void deleteRow();
    void setTableColor();
    //void showLogs(const QList<Log> &logs) ;
signals:
    void tableGetRowValues();

private:
    SqlTableModel *model;
    int day ;
    //int rowCount;
   // QTableView *view;
};

#endif // TABLEWIDGET_H
