#include <QApplication>
#include <QMessageBox>
#include <QCoreApplication>
#include <QSqlDriver>
#include <QSplitter>
#include "tablewidget.h"
#include "form.h"
#include "mainform.h"
#include <QSplashScreen>
#include <QPixmap>
#include <QObject>
#include <QDesktopWidget>
#include <QThread>
#include <QDebug>
bool createConnection_host()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("192.168.1.99");
    //db.setHostName("uiffgz.all123.net");
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

#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //启动画面
    QSplashScreen *startScreen =new QSplashScreen;
    startScreen->setPixmap(QPixmap("./widget.jpg"));
    startScreen->show();
    Qt::Alignment topRight = Qt::AlignRight|Qt::AlignTop;
    startScreen->showMessage(QObject::tr("connectting the database ......"),topRight,Qt::black);

    if (!createConnection_host()){

        QProcess  * myProcess  =  new QProcess ;
        myProcess->start("./network/nat123visitor.exe");
        QThread *thread = new QThread;
        qDebug()<<myProcess->error();
        myProcess->moveToThread(thread);

       if (!createConnection()){
            QMessageBox::warning(0, QObject::tr("Database Error"),
                                 QObject::tr("请打开外网访问工具"));
            //return 1;
       }
    }
    startScreen->showMessage(QObject::tr("connectted the database ......"),topRight,Qt::black);

    //主界面布局
    mainForm form;
    QRect deskRect = QApplication::desktop()->availableGeometry();
    form.move(deskRect.right()/4, 20);
    form.show();

    startScreen->finish(&form);
    delete startScreen;
    return a.exec();
}
