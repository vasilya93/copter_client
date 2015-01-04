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
    QCustomPlot *m_plotAcc;
    QCustomPlot *m_plotGyro;
    QPushButton *m_pButtonSendStart;

    QTimer *m_pTimerRedraw;
    QVector<double> m_vecX;
    QVector<double> m_vecXGyro;
    QVector<double> m_vecAccX;
    QVector<double> m_vecAccY;
    QVector<double> m_vecAccZ;
    QVector<double> m_vecGyroX;
    QVector<double> m_vecGyroY;
    QVector<double> m_vecGyroZ;
    double m_fCurrentX,
           m_fCurrentXGyro;
    CommManager *m_pCommManager;
};

#endif // CLIENTVIEW_H
