#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include <QGridLayout>
#include <QSplitter>
#include <QToolBar>
#include <QPushButton>
#include <QFontComboBox>

class mainForm : public QWidget
{
    Q_OBJECT

public:
    mainForm(QWidget *parent = 0);
    ~mainForm();
private:
    QGridLayout *mainLoyout;
    QHBoxLayout *hlayout ;
    QFontComboBox *fontComboBox;
    QToolBar *filetoolbar ;
    QPushButton *pushButton ;
    QSpacerItem *horizontalSpacer ;
    //QSplitter *splitter;
};

#endif // MAINFORM_H
