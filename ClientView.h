#ifndef CLIENTVIEW_H
#define CLIENTVIEW_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPushButton>
#include "qcustomplot.h"
#include "commmanager.h"

class ClientView : public QWidget
{
    Q_OBJECT

public:
    ClientView(QWidget *parent = 0);
    virtual ~ClientView();

public slots:
    void slotSendStart();
    void slotUpdateGraphView();
    void slotUpdateAccVectors(int nAccX, int nAccY, int nAccZ);
    void slotUpdateGyroVectors(int nGyroX, int nGyroY, int nGyroZ);

private:
    QCustomPlot *m_plot;
    QPushButton *m_pButtonSendStart;

    QTimer *m_pTimerRedraw;
    QVector<double> m_vecX;
    QVector<double> m_vecY1;
    QVector<double> m_vecY2;
    QVector<double> m_vecY3;
    double m_fCurrentX;
    CommManager *m_pCommManager;
};

#endif // CLIENTVIEW_H
