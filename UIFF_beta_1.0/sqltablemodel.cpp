#include "sqltablemodel.h"
#include <QColor>
#include <QBrush>
#include <QSqlRecord>
//Reimplemented from QSqlTableModel::data , want to add color for every row;
SqlTableModel::SqlTableModel(QObject *parent ):QSqlTableModel(parent)
{

}
SqlTableModel::~SqlTableModel(){

}
QVariant SqlTableModel::data(const QModelIndex &index, int role) const
{
if (Qt::BackgroundColorRole == role)
    {
            QSqlRecord record = this->record(index.row());
            int colum = this->columnCount();
            QString tmp = record.value(colum-1).toString();
            if( tmp != "0"&&tmp != "#000000"&&tmp !="#ffffff"&&tmp!="")
            {
                return 	QBrush(QColor(tmp));
            }
    }
    return QSqlTableModel::data(index, role);
}
