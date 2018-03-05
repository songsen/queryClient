#include "form.h"
#include "ui_form.h"
#include "signalapp.h"
//#include <QDebug>
#include <QSqlQuery>

extern signalApp *sig;

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);

    connect(this,SIGNAL(formSignal_ok(int)),sig,SLOT(connectSlots_ok(int)));
    connect(this,SIGNAL(formSignal_combobox(int)),sig,SLOT(connectSlots_comobox(int)));
    connect(this,SIGNAL(formSignal_calendar(int)),sig,SLOT(connectSlots_calendar(int)));
    connect(sig,SIGNAL(connectSignals_table()),this,SLOT(fromTableSetTextEdit()));
    connect(this,SIGNAL(formSignal_add()),sig,SLOT(connectSlots_add()));
    connect(this->ui->pushButton_delete,SIGNAL(clicked(bool)),sig,SLOT(FromForm_delete()));//改进后的
    connect(this->ui->toolButton_color,SIGNAL(clicked(bool)),sig,SLOT(fromFrom_color()));
}

Form::~Form()
{
    delete ui;
}



void Form::on_pushButton_ok_clicked()
{
    sig->sendData.from = ui->lineEdit_from->text();
    sig->sendData.to = ui->lineEdit_to->text();
    sig->sendData.carrier = ui->lineEdit_carrier->text();
    sig->sendData.region = ui->comboBox->currentIndex();

    emit formSignal_ok(0);
}

void Form::on_comboBox_currentIndexChanged(int index)
{
    sig->sendData.region = ui->comboBox->currentIndex();
    emit formSignal_combobox(index);
}

void Form::on_calendarWidget_clicked(const QDate &date)
{
    emit formSignal_calendar(date.toString().toInt());
}
void Form::fromTableSetTextEdit(){
    ui->textBrowser->clear();
    ui->textBrowser->setText(sig->sendData.tableRowDataList.join("-"));

    ui->lineEdit_from->setText(sig->sendData.from);
    ui->lineEdit_to->setText(sig->sendData.to);
   // ui->comboBox->setCurrentIndex(sig->sendData.region);

}

void Form::on_pushButton_clear_clicked()
{
    ui->lineEdit_carrier->clear();
    ui->lineEdit_from->clear();
    ui->lineEdit_to->clear();
    sig->sendData.from = "";
    sig->sendData.to = "";
    sig->sendData.carrier = "";
    sig->sendData.country = "";
    sig->sendData.localcharge = "";
    sig->sendData.tableRowDataList.empty();
    sig->sendData.region = 0;
    on_pushButton_ok_clicked();
}

void Form::on_pushButton_add_clicked()
{
    emit formSignal_add();
}

//void Form::on_pushButton_delete_clicked()
//{
//    emit formSignal_delete();
//}
void Form::displayUserList(){

    QSqlQuery *query = new QSqlQuery ;
    query->exec("show full processlist");
    if(query->size()>0){
        while(query->next()){
                ui->textBrowser->append(query->value(2).toString());
        }
    }
    ui->textBrowser->append("UIIF Shipping Line\n");
    delete query;
}
