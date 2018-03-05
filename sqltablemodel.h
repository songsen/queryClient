#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModel:public QSqlTableModel
{
public:
    SqlTableModel(QObject *parent = Q_NULLPTR);
    ~SqlTableModel();
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // SQLTABLEMODEL_H
