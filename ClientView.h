#ifndef CLIENTVIEW_H
#define CLIENTVIEW_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include "qcustomplot.h"
#include "commmanager.h"
#include "orientationindicator.h"

class ClientView : public QWidget
{
    Q_OBJECT

public:
    ClientView(QWidget *parent = 0);
    virtual ~ClientView();

public slots:
    void slotSendStart();
    void slotUpdateGraphView();

private:
    void AddValuesToAccVector(int nAccX, int nAccY, int nAccZ);
    void AddValuesToGyroVector(int nGyroX, int nGyroY, int nGyroZ);
    void AddValuesToAccVector(QVector<double> vecAccX,
                              QVector<double> vecAccY,
                              QVector<double> vecAccZ);
    void AddValuesToGyroVector(QVector<double> vecGyroX,
                               QVector<double> vecGyroY,
                               QVector<double> vecGyroZ);

private:
    QLabel *m_pLabelTimeGap;
    QCustomPlot *m_plotAcc;
    QCustomPlot *m_plotGyro;
    QTextEdit *m_editDCM;
    QPushButton *m_pButtonSendStart;
    OrientationIndicator *m_pIndicatorRoll,
                         *m_pIndicatorPitch,
                         *m_pIndicatorYaw;
    QTimer *m_pTimerRedraw;

    QVector<double> m_vecXAccX;
    QVector<double> m_vecXAccY;
    QVector<double> m_vecXAccZ;

    QVector<double> m_vecXGyroX;
    QVector<double> m_vecXGyroY;
    QVector<double> m_vecXGyroZ;

    QVector<double> m_vecAccX;
    QVector<double> m_vecAccY;
    QVector<double> m_vecAccZ;
    QVector<double> m_vecGyroX;
    QVector<double> m_vecGyroY;
    QVector<double> m_vecGyroZ;
    double m_fCurrentXAccX,
           m_fCurrentXAccY,
           m_fCurrentXAccZ,
           m_fCurrentXGyroX,
           m_fCurrentXGyroY,
           m_fCurrentXGyroZ;
    CommManager *m_pCommManager;

    bool m_bDoAllowEdit;

    const int SIZE_SET = 500;
};

#endif // CLIENTVIEW_H
