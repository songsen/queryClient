#include "mainform.h"
#include <QGridLayout>
#include <QSplitter>
#include "tablewidget.h"
#include <QSplitter>
#include <QT>
#include <QFontComboBox>
#include <QPushButton>
#include <QToolBar>

mainForm::mainForm(QWidget *parent)
    : QWidget(parent)
{

    mainLoyout = new QGridLayout;
    fontComboBox = new QFontComboBox;
    filetoolbar = new QToolBar;
    filetoolbar->addAction(QIcon("d:/widget.jpg"),"open");
    pushButton = new QPushButton("ok");
    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mainLoyout->addWidget(fontComboBox,0,0);
    mainLoyout->setHorizontalSpacing(mainLoyout->horizontalSpacing());
    mainLoyout->addWidget(pushButton,0,1);
    mainLoyout->addWidget(filetoolbar,0,2);
    setLayout(mainLoyout);
    setContentsMargins(0,0,0,0);//设置边距
    mainLoyout->setContentsMargins(0,0,0,0);//设置行距
    //pushButton->setContentsMargins(0,0,0,0);
}

mainForm::~mainForm(){

}
