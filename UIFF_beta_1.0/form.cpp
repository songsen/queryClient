#include "form.h"
#include "ui_form.h"
//#include <QDebug>
#include <QSqlQuery>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    displayUserList();
}

Form::~Form()
{
    delete ui;
}
void Form::setButtonInvalid(){
    ui->pushButton_ok->setEnabled(false);
    ui->pushButton_add->setEnabled(false);
    ui->pushButton_clear->setEnabled(false);
    ui->pushButton_delete->setEnabled(false);
    ui->comboBox->setEditable(false);
}
void Form::setButtonValid(){
        ui->pushButton_ok->setEnabled(true);
        ui->pushButton_add->setEnabled(true);
        ui->pushButton_clear->setEnabled(true);
        ui->pushButton_delete->setEnabled(true);
        ui->comboBox->setEditable(true);
}

void Form::on_calendarWidget_clicked(const QDate &date)
{

}

void Form::on_pushButton_clear_clicked()
{
    ui->lineEdit_carrier->clear();
    ui->lineEdit_from->clear();
    ui->lineEdit_to->clear();   
}

void Form::displayUserList(){

    QSqlQuery *query = new QSqlQuery ;
    query->exec("show full processlist");
    if(query->size()>0){
        while(query->next()){
                ui->textBrowser->append(query->value(2).toString());
        }
    }
    ui->textBrowser->append(QObject::tr("1.已启用模糊搜索\n"));
    ui->textBrowser->append("UIIF Shipping Line\n");
    delete query;
}
