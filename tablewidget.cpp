#include "tablewidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QHeaderView>
#include <QToolTip>
//#include <QDebug>
#include "signalapp.h"
#include <QSqlRecord>
#include <QMessageBox>
#include <QColorDialog>
#include <QSqlQuery>

enum  HEADER_LABEL{
    id ,
    Country,
    LoadPort,
    UnloadPort,
    Carrier,
    Size20GP,
    Size40GP,
    Size40HQ ,
    Size45HQ ,
    CloseDate,
    LocalCharge,
    Voyage ,
    Enterport,
    ValidStartDate,
    ValidEndDate ,
    Note ,
    Consultant ,
    region ,
    modifytime ,
    modifydate,
    Color
};

const QString QStringTableName = "uiffshippingline" ;
const QString QStringLoadPort = "loadport";
const QString QStringUnloadPort = "unloadport";
const QString QStringCarrier = "carrier";

extern signalApp *sig;

void tablewidget::showToolTip(const QModelIndex &index)//显示气泡提示
{
  if(!index.isValid())
    return;
  //int row = index.row();
  QVariant var= model->data(index);
  QString str = var.toString();
  if(str.isEmpty())
    return;
  QToolTip::showText(QCursor::pos(), str);
}

void tablewidget::findTable(){
    QString tmp;
    bool b=0;
     if(sig->sendData.from !=""){
        tmp.append(QObject::tr("LoadPort LIKE '%1%'").arg(sig->sendData.from));
        b = 1;
     }
     if(sig->sendData.to != ""){
         if(b) tmp.append(QObject::tr(" and "));
         tmp.append(QObject::tr("UnloadPort LIKE '%1%'").arg(sig->sendData.to));
         b=1;
     }
     if(sig->sendData.carrier !=""){
         if(b) tmp.append(QObject::tr(" and "));
         tmp.append(QObject::tr("carrier='%1%'").arg(sig->sendData.carrier));
         b=1;
     }
     if(sig->sendData.region !=0){
         if(b) tmp.append(QObject::tr(" and "));
         tmp.append(QObject::tr("region='%1'").arg(sig->sendData.region));
     }
     if(!tmp.isEmpty()){
        model->setFilter(tmp);
             model->setSort(1, Qt::AscendingOrder);//以列排序
             model->select();
             return ;
     }
     else{
         model->setTable(QStringTableName);
         model->select();//先执行select之后再执行settablefrom格式才有效；
         setTableFrom();
         //model->select();
         return;
     }
}
inline void tablewidget::setTableFrom(){

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setColumnHidden(0, true);//隐藏0列
   // this->setColumnHidden(17,true);
    this->setColumnHidden(18,true);
    this->setColumnHidden(19,true);
    this->setColumnHidden(HEADER_LABEL::Color,true);
    this->resizeColumnsToContents();
    this->setEditTriggers(QAbstractItemView::DoubleClicked);
    this->verticalHeader()->setVisible(false);//垂直表头不可见

    QHeaderView *header = this->horizontalHeader();
    header->setStretchLastSection(true);
    //设置表头
    //queryModelGoods.sort(0);
    //SortIndicator为水平标题栏文字旁边的三角指示器
    header->setSortIndicator(0, Qt::AscendingOrder);
    header->setSortIndicatorShown(true);
   // header->setClickable(true);
    header->setSectionsClickable(true);
    connect(header, SIGNAL(sectionClicked(int)), this, SLOT (sortByColumn(int)));

}

tablewidget::tablewidget(QWidget *parent ):QTableView(parent)
{
   // rowCount = 0;
    QSqlQuery query;
    //获得系统时间转换为整数形式
    query.exec("select curdate()+0");
    query.next();
    day = query.value(0).toInt();

    model = new SqlTableModel;
    model->setTable("uiffshippingline");
    model->setSort(1, Qt::AscendingOrder);//以列排序
    model->select();

    setMouseTracking(true);
    this->setModel(model);//绑定模型

    setTableFrom();//设置表格格式
    QString s=    "QTableView::item:selected{color:white;background:rgb(34, 175, 75);  }";
    this->setStyleSheet(s);
     setAlternatingRowColors(  true );

    connect(sig,SIGNAL(connectSignals_comobox(int)),this,SLOT(comoboxTable(int)));
    connect(this, &tablewidget::entered, this, &tablewidget::showToolTip); //实现鼠标跟随现实
    connect(sig,SIGNAL(connectSignals_ok(int)),this,SLOT(findTable()));
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(tableClicked(QModelIndex)));
    connect(this,SIGNAL(tableGetRowValues()),sig,SLOT(connectSlots_Table()));
    connect(sig,SIGNAL(connectSignals_add()),this,SLOT(sqlModelAdd()));
    connect(sig,SIGNAL(ToTable_delete()),this,SLOT(deleteRow()));
    connect(sig,SIGNAL(ToTable_color()),this,SLOT(setTableColor()));

}
void tablewidget::deleteRow(){
   QMessageBox::StandardButton choice = QMessageBox::warning(this,QObject::tr("Warning") ,
                         QObject::tr("Do you sure delete this cell row!"),QMessageBox::Yes|QMessageBox::No);
    if(choice == QMessageBox::StandardButton::Yes){
        model->removeRow(sig->sendData.index.row());
        model->submitAll();
        model->select();
    }
    else{
        return;
    }
}

tablewidget::~tablewidget(){
    delete model;
   // delete view;
}
void tablewidget::comoboxTable(int s){
   // qDebug()<<s;
    model->setFilter(QString("region='%1'").arg(s));
    model->select();
}
void tablewidget::tableClicked(QModelIndex index){
     QSqlRecord record = model->record(index.row());
     int colum = model->columnCount();
     int count =0;

     sig->sendData.index = index;
     sig->sendData.country = record.value(1).toString();
     sig->sendData.from = record.value(2).toString();
     sig->sendData.to = record.value(3).toString();
     sig->sendData.carrier = record.value(4).toString();
     sig->sendData.region = record.value(17).toInt();
     sig->sendData.localcharge = record.value(10).toString();

     sig->sendData.tableRowDataList.clear();
    while(colum){
        QString tmp = record.value(count).toString();
        if(tmp !=""){
            sig->sendData.tableRowDataList <<tmp;
        }
        count++;
        colum--;
    }
    emit tableGetRowValues();
}
void tablewidget::sqlModelAdd(){
    int row = 0;
    model->insertRow(row);
    model->setData(model->index(row, HEADER_LABEL::Country),sig->sendData.country);
    model->setData(model->index(row, HEADER_LABEL::LoadPort),sig->sendData.from);
    model->setData(model->index(row, HEADER_LABEL::UnloadPort),sig->sendData.to);
    model->setData(model->index(row, HEADER_LABEL::LocalCharge),sig->sendData.localcharge);
    model->setData(model->index(row, HEADER_LABEL::region),sig->sendData.region);
    model->setData(model->index(row, HEADER_LABEL::modifydate),day);
    model->submitAll();
    model->select();
}
void tablewidget::setTableColor(){

      // QColorDialog * ColorDialog = new QColorDialog;

       QColor color = QColorDialog::getColor(Qt::white, this);
       QString tmp = color.name();
        if(tmp != "#000000"){
            model->setData(model->index(sig->sendData.index.row(),HEADER_LABEL::Color),tmp);
            model->submitAll();
        }

       // qDebug()<<color.name();
        //model->item(0, 4)->setForeground(QBrush(QColor(255, 0, 0)));
       // model->itemData(sig->sendData.index).
}
//void tablewidget::showLogs(const QList<Log> &logs) {

//ModelBuilder::buildModel(&(this->model), logs);
//    this->setModel(this->model);
//    this->resizeColumnsToContents();
//    connect(ui->tableView->selectionModel(),
//    SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
//    this, SLOT(showOperationDetails(const QModelIndex &, const QModelIndex &)));

//}
