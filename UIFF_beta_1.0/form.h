#ifndef FORM_H
#define FORM_H
#include "ui_form.h"
#include <QWidget>


namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    void displayUserList();
          Ui::Form *ui;

public slots:
    void on_pushButton_clear_clicked();
    void on_calendarWidget_clicked(const QDate &date);
    void setButtonInvalid();
    void setButtonValid();

protected:


private:

};

#endif // FORM_H
