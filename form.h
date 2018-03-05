#ifndef FORM_H
#define FORM_H

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

private slots:
    void on_pushButton_ok_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_calendarWidget_clicked(const QDate &date);

    void fromTableSetTextEdit();

    void on_pushButton_clear_clicked();

    void on_pushButton_add_clicked();

 //   void on_pushButton_delete_clicked();

  //  void on_toolButton_color_clicked();

signals:
    void formSignal_ok(int);
    void formSignal_combobox(int);
    void formSignal_calendar(int);
    void formSignal_add();
//    void formSignal_delete();

private:
    Ui::Form *ui;
};

#endif // FORM_H
