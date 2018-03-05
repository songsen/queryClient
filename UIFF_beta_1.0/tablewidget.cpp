#include "tablewidget.h"
#include <QTableView>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QHeaderView>
#include <QToolTip>
//#include <QDebug>
#include <QSqlRecord>
#include <QMessageBox>
#include <QColorDialog>
#include <QSqlQuery>
#include <QThread>

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

tablewidget::tablewidget(QWidget *parent ):QTableView(parent)
{
//    QThread *threadModel = new QThread;
//    this->moveToThread(threadModel);
//    threadModel->start();
    runOrStop = 0;
    this->setMouseTracking(true);//探测鼠标位置
    this->setModel(model);
    setTableFrom();//设置表格格式
    model->select();
    connect(this, &tablewidget::entered, this, &tablewidget::showToolTip); //实现鼠标跟随现实
}
tablewidget::tablewidget(QWidget *parent,SqlTableModel *mod):QTableView(parent),model(mod){
    this->setMouseTracking(true);//探测鼠标位置
    this->setModel(model);
    model->select();
    setTableFrom();//设置表格格式
    connect(this,SIGNAL(clicked(QModelIndex)),this,SLOT(clickedChoiceRow(QModelIndex)));
    connect(this, &tablewidget::entered, this, &tablewidget::showToolTip); //实现鼠标跟随现实

}

tablewidget::~tablewidget(){
    delete model;
}

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

void tablewidget::tableFrom(){
    this->setColumnHidden(0, true);//隐藏0列
  // this->setColumnHidden(17,true);
   this->setColumnHidden(18,true);
   this->setColumnHidden(19,true);
   this->setColumnHidden(HEADER_LABEL::Color,true);
    this->resizeColumnsToContents();
    model->setSort(1, Qt::AscendingOrder);//以列排序
}

void tablewidget::setTableFrom(){
    QString s=    "QTableView::item:selected{color:white;background:rgb(34, 175, 75);  }";
    this->setStyleSheet(s);
    setAlternatingRowColors(  true );//交替纹格

    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
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
    tableFrom();
    connect(header, SIGNAL(sectionClicked(int)), this, SLOT (sortByColumn(int)));
}




void tablewidget::clickedChoiceRow(QModelIndex index){
    mutex.lock();
     QSqlRecord record = model->record(index.row());
     //int colum = model->columnCount();


     sendData.index = index;
     sendData.country = record.value(1).toString();
     sendData.from = record.value(2).toString();
     sendData.to = record.value(3).toString();
     sendData.carrier = record.value(4).toString();
     sendData.region = record.value(17).toInt();
     sendData.localcharge = record.value(10).toString();
     sendData.row = index.row();

     sendData.tableRowDataList.clear();

    for(int count =1;count<Consultant;count++)
    {
        QString tmp = record.value(count).toString();
        if(tmp !=""&&tmp!="0"){
            this->sendData.tableRowDataList <<tmp;
        }
    }
    mutex.unlock();
    emit ChoiceRowSignal();
}

void tablewidget::setRowColor(){
      emit running();
       QColor color = QColorDialog::getColor(Qt::white, this);
       QString tmp = color.name();
        if(tmp != "#000000"){
            model->setData(model->index(sendData.index.row(),HEADER_LABEL::Color),tmp);
            model->submitAll();
        }
    emit finished();
}


void tablewidget::deleteRowFromSQL(){
    if(this->sendData.row == -1){
        QMessageBox::warning(this,QObject::tr("warning"),QObject::tr("请选择要删除的行！"));
        return;
    }
    QMessageBox::StandardButton choice = QMessageBox::warning(this,QObject::tr("Warning") ,
    QObject::tr("你确定要删掉%1行吗!").arg(this->sendData.row),QMessageBox::Yes|QMessageBox::No);
     if(choice == QMessageBox::StandardButton::Yes){
         model->removeRow(this->sendData.row);
         this->sendData.row = -1;
         model->submitAll();
         model->select();

     }
     else{
         return;
     }
     emit finished();
}

void tablewidget::addRowToSQL(){
    emit running();
    int row = 0;
    model->insertRow(row);
    model->setData(model->index(row, HEADER_LABEL::Country),this->sendData.country);
    model->setData(model->index(row, HEADER_LABEL::LoadPort),this->sendData.from);
    model->setData(model->index(row, HEADER_LABEL::UnloadPort),this->sendData.to);
    model->setData(model->index(row, HEADER_LABEL::LocalCharge),this->sendData.localcharge);
    model->setData(model->index(row, HEADER_LABEL::region),this->sendData.region);
    model->setData(model->index(row, HEADER_LABEL::modifydate),date);
    model->submitAll();
    model->select();
    emit finished();
}
void tablewidget::setCurrentDate(int i){
    date = i;
}
void tablewidget::setTableModel(SqlTableModel *mod){
    model = mod;
}
