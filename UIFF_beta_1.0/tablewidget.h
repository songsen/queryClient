#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>
#include "sqltablemodel.h"
#include <QMutex>

class trailerlistWidget:public QWidget
{
     Q_OBJECT
public:
     trailerlistWidget(QWidget *parent = 0):QWidget(parent){
         model = new QSqlTableModel;
         trailerlistView = new QTableView;
         model->setTable("trailerlist");
         model->select();
         trailerlistView->setModel(model);
     }
     void addRowToSQL(){
         int row = 0;
         model->insertRow(row);
         model->submitAll();
         model->select();
     }

     ~trailerlistWidget(){

     }
private:
     QTableView *trailerlistView ;
     QSqlTableModel *model;
};


class tablewidget : public QTableView
{
    Q_OBJECT

public:
    tablewidget(QWidget *parent=0);
    tablewidget(QWidget *parent=0,SqlTableModel *lefttablewidget=0);
    ~tablewidget();
virtual void setTableFrom();
    void tableFrom();
    void setCurrentDate(int);
    void setTableModel(SqlTableModel *mod);
    struct data{
        QString country;
        QString from;
        QString to;
        QString carrier;
        QStringList tableRowDataList;
        QModelIndex index;
        QString localcharge;
        int row;
        int region;
    };
    struct data sendData ;

public slots:
    void showToolTip(const QModelIndex &index);
    //virtual void findFromSql();
    void clickedChoiceRow(QModelIndex index);
    void setRowColor();
    void deleteRowFromSQL();
    void addRowToSQL();
signals:
    void ChoiceRowSignal();
    void running();
    void finished();

private:
    SqlTableModel *model;
    int date ;
    QMutex mutex;
    bool runOrStop;
};

#endif // TABLEWIDGET_H

