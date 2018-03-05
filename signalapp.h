#ifndef SIGNALAPP_H
#define SIGNALAPP_H

#include <QObject>
#include <QModelIndex>

class signalApp : public QObject
{
    Q_OBJECT
public:
    explicit signalApp(QObject *parent = 0);
    ~signalApp();

    struct data{
        QString country;
        QString from;
        QString to;
        QString carrier;
        QStringList tableRowDataList;
        QModelIndex index;
        QString localcharge;
        int region;
    };
    struct data *sendData;
signals:
    void connectSignals_ok(int);
    void connectSignals_comobox(int);
    void connectSignals_calendar(int);
    void connectSignals_table();
    void connectSignals_add();
    void ToTable_delete();
    void ToTable_color();

public slots:
    void connectSlots_ok(int);
    void connectSlots_comobox(int);
    void connectSlots_calendar(int);
    void connectSlots_Table();
    void connectSlots_add();
    void FromForm_delete();
    void fromFrom_color();
};

#endif // SIGNALAPP_H
