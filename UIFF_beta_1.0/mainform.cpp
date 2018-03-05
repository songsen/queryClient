#include "mainform.h"
#include <QGridLayout>
#include <QSplitter>
#include "tablewidget.h"
#include <QSplitter>
#include <QFontComboBox>
#include <QPushButton>
#include <QToolBar>
#include <QSqlQuery>
#include <QMessageBox>
#include <QColorDialog>
#include <QDebug>
#include <QThread>
#include "plotter.h"

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

const QString StringTableName = "uiffshippingline";

mainForm::mainForm(QWidget *parent )
    :QTabWidget(parent)
{
    //数据库模型
    model = new SqlTableModel;
    model->setTable("uiffshippingline");
    model->setSort(1, Qt::AscendingOrder);//以列排序
    //UI界面布局
    splitter = new QSplitter(Qt::Horizontal);
    lefttablewidget = new tablewidget(splitter,model);
    rightForm = new Form(splitter);
   // setOrientation(Qt::Horizontal);//垂直切割窗体
    rightForm->setFixedWidth(rightForm->sizeHint().width()/1.2);//右边最优宽度/1.2
    splitter->setStretchFactor(0,8);//比例切分
    splitter->setStretchFactor(1,1);
    splitter->resize(600,450);//初始化大小
     lefttablewidget->sendData.row = -1;
     lefttablewidget->sendData.region=0;
     this->trailer = new trailerlistWidget;

    addTab(splitter,QObject::tr("ShippingLine"));
    addTab(trailer,QObject::tr("拖车报关"));


    QThread *threadModel = new QThread;
    model->moveToThread(threadModel);
    threadModel->start();
    //获取服务器时间
    QSqlQuery query;
    query.exec("select curdate()+0");
    query.next();
    date = query.value(0).toInt();

    lefttablewidget->setCurrentDate(date);

    setWindowTitle(QObject::tr("UIFFGZ"));
    //setWindowIcon(QIcon(QPixmap("/image/uiff.png")));

    connect(rightForm->ui->comboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(comboxIndexChanged(int )));
    connect(rightForm->ui->pushButton_ok,SIGNAL(clicked(bool)),this,SLOT(findFromSql()));
    connect(lefttablewidget,SIGNAL(ChoiceRowSignal()),this,SLOT(fillTextEditFromTable()));
    connect(rightForm->ui->toolButton_color,SIGNAL(clicked(bool)),lefttablewidget,SLOT(setRowColor()));
    connect(rightForm->ui->pushButton_add,SIGNAL(clicked(bool)),lefttablewidget,SLOT(addRowToSQL()));
    connect(rightForm->ui->pushButton_delete,SIGNAL(clicked(bool)),lefttablewidget,SLOT(deleteRowFromSQL()));
    connect(lefttablewidget,SIGNAL(running()),rightForm,SLOT(setButtonInvalid()));
    connect(lefttablewidget,SIGNAL(finished()),rightForm,SLOT(setButtonValid()));
    connect(rightForm->ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(setPlotter()));

}

mainForm::~mainForm(){
    delete rightForm;
    delete lefttablewidget;
    delete model;
    delete plotter;
    delete trailer;
}

void mainForm::setPlotter(){
    plotter = new Plotter;
    plotter->setWindowModality(Qt::ApplicationModal);//模态对话框
    plotter->setWindowTitle(QObject::tr("历史曲线"));
    //int numPoints = 100;
    QVector<QPointF> points0;
   // QVector<QPointF> points1;

    QSqlQuery query;
    query.exec("select id,size40hq from uiffshippingline ");
    query.next();
    int minx = query.value(0).toInt();
    int maxx =0;
    int miny=query.value(1).toInt();
    int maxy = 0;
    do{
         maxx = query.value(0).toInt();
         maxy = query.value(1).toInt();
         points0.append(QPointF(maxx, maxy));
    }
    while(query.next());
        plotter->setCurveData(0, points0);
   /*
    for (int x = 0; x < numPoints; ++x) {
        points0.append(QPointF(x, uint(qrand()) % 100));
        points1.append(QPointF(x, uint(qrand()) % 100));
    }

    plotter->setCurveData(1, points1);*/

    PlotSettings settings;
    settings.minX = minx;
    settings.maxX = maxx;
    settings.minY = miny;
    settings.maxY = maxy;
    plotter->setPlotSettings(settings);
    //plotter.setModal(true);
    plotter->show();
}

void mainForm::comboxIndexChanged(int index){
    rightForm->setButtonInvalid();
    model->setFilter(QString("region='%1'").arg(index));
    model->select();
    this->lefttablewidget->tableFrom();
    rightForm->setButtonValid();
}

void mainForm::findFromSql(){
    bool b=0;
    rightForm->setButtonInvalid();
    QString from = rightForm->ui->lineEdit_from->text();
    QString to = rightForm->ui->lineEdit_to->text();
    QString carrier = rightForm->ui->lineEdit_carrier->text();
    int region = rightForm->ui->comboBox->currentIndex();

    QString tmp;
    if(from !=""){
        tmp.append(QObject::tr("LoadPort LIKE '%1%'").arg(from));
        b = 1;
    }
    if(to != ""){
     if(b) tmp.append(QObject::tr(" and "));
         tmp.append(QObject::tr("UnloadPort LIKE '%1%'").arg(to));
         b=1;
    }
    if(carrier !=""){
         if(b) tmp.append(QObject::tr(" and "));
         tmp.append(QObject::tr("carrier LIKE '%%1%'").arg(carrier));
         b=1;
    }
    if(region !=0){
         if(b) tmp.append(QObject::tr(" and "));
         tmp.append(QObject::tr("region='%1'").arg(region));
    }
    if(!tmp.isEmpty()){

        model->setFilter(tmp);
        model->setSort(1, Qt::AscendingOrder);//以列排序
        model->select();

    }
    else{
         model->setTable(StringTableName);
         model->select();//先执行select之后再执行settablefrom格式才有效；
         lefttablewidget->setTableFrom();
     //model->select();

    }
    rightForm->setButtonValid();
}

void mainForm::fillTextEditFromTable(){
    rightForm->ui->textBrowser->clear();
    rightForm->ui->textBrowser->setText(lefttablewidget->sendData.tableRowDataList.join("-"));
    rightForm->ui->lineEdit_from->setText(lefttablewidget->sendData.from);
    rightForm->ui->lineEdit_to->setText(lefttablewidget->sendData.to);
}
