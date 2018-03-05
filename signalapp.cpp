#include "signalapp.h"
#include <QMessageBox>

signalApp::signalApp(QObject *parent) : QObject(parent)
{

}

signalApp::~signalApp()
{

}

void signalApp::connectSlots_ok(int s){
//    QStringList str;
//    str<<sendData.carrier<<sendData.from<<sendData.to;
//     qDebug()<<m<<":"<<str;
    emit connectSignals_ok(s);
}
void signalApp::connectSlots_calendar(int s){
    emit connectSignals_calendar(s);
}
void signalApp::connectSlots_comobox(int s){
    emit connectSignals_comobox(s);
}
void signalApp::connectSlots_Table(){
    emit connectSignals_table();
}
void signalApp::connectSlots_add(){
    emit connectSignals_add();
}
void signalApp::FromForm_delete(){
    emit ToTable_delete();
}
void signalApp::fromFrom_color(){
    emit this->ToTable_color();
}
