#include <QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSqlDriver>
#include <QSplitter>
#include "tablewidget.h"
#include "form.h"
#include "signalapp.h"
#include "mainform.h"
#include <QSplashScreen>
#include <QPixmap>
#include <QObject>
#include <QDesktopWidget>

signalApp *sig;

bool createConnection_host()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.1.99");
    db.setDatabaseName("uiff");
    db.setUserName("root");
    db.setPassword("mysql");
    if(!db.open()){
       // qDebug() <<"data dirve load"<<db.lastError().text()<<endl;
        return false;
    }
    return true;
}

bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("uiffgz.all123.net");
    //db.setHostName("192.168.1.99");
    db.setDatabaseName("uiff");
    db.setUserName("root");
    db.setPassword("mysql");
    if(!db.open()){
       // qDebug() <<"data dirve load"<<db.lastError().text()<<endl;
        return false;
    }
    return true;
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //启动画面
    QSplashScreen *startScreen =new QSplashScreen;
    startScreen->setPixmap(QPixmap("/widget.jpg"));
    startScreen->show();
    Qt::Alignment topRight = Qt::AlignRight|Qt::AlignTop;
    startScreen->showMessage(QObject::tr("connectting the database ......"),topRight,Qt::black);

    if (!createConnection_host()){
       if (!createConnection()){
            QMessageBox::warning(0, QObject::tr("Database Error"),
                                 QObject::tr("请打开外网访问工具"));
            //return 1;
       }
    }


    startScreen->showMessage(QObject::tr("connectted the database ......"),topRight,Qt::black);

    //信号通信连接器
    signalApp mainSignal;
    sig = &mainSignal;
    QSplitter *splitter = new QSplitter(Qt::Horizontal);
    //添加右侧界面
    Form *topWidget = new Form;
    topWidget->setMaximumWidth(100);
    topWidget->setFixedWidth(topWidget->sizeHint().width());
    //添加左边表格
    tablewidget *tableWinodw = new tablewidget;
    //主界面显示在屏幕中的位置布局
    QRect deskRect = QApplication::desktop()->availableGeometry();
    splitter->move(deskRect.right()/4, 20);

    splitter->addWidget(tableWinodw);
    splitter->addWidget(topWidget);
    splitter->setStretchFactor(0,8);
    splitter->setStretchFactor(1,1);
    splitter->resize(600,450);
    splitter->show();

    topWidget->displayUserList();

    startScreen->finish(splitter);
    delete startScreen;
    return a.exec();
}
