#include <QVBoxLayout>
#include <math.h>
#include "ClientView.h"

ClientView::ClientView(QWidget *parent)
    : QWidget(parent),
      m_fCurrentX(0),
      m_fCurrentXGyro(0)
{
    m_plotAcc = new QCustomPlot;
    m_plotGyro = new QCustomPlot;
    m_pTimerRedraw = new QTimer;
    m_pCommManager = new CommManager;

    m_vecX.append(m_fCurrentX);
    m_vecXGyro.append(m_fCurrentXGyro);
    m_vecAccX.append(0);
    m_vecAccY.append(0);
    m_vecAccZ.append(0);
    m_vecGyroX.append(0);
    m_vecGyroY.append(0);
    m_vecGyroZ.append(0);
    m_fCurrentX++;
    m_fCurrentXGyro++;

    m_plotAcc->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    m_plotAcc->legend->setFont(legendFont);
    m_plotAcc->legend->setBrush(QBrush(QColor(255,255,255,230)));
    m_plotAcc->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    m_plotAcc->addGraph();
    m_plotAcc->graph(0)->setPen(QPen(QColor(Qt::blue)));
    m_plotAcc->graph(0)->setName("Acceleration X");
    m_plotAcc->graph(0)->setData(m_vecX, m_vecAccX);
    m_plotAcc->addGraph();
    m_plotAcc->graph(1)->setPen(QPen(QColor(Qt::red)));
    m_plotAcc->graph(1)->setName("Acceleration Y");
    m_plotAcc->graph(1)->setData(m_vecX, m_vecAccY);
    m_plotAcc->addGraph();
    m_plotAcc->graph(2)->setPen(QPen(QColor(Qt::green)));
    m_plotAcc->graph(2)->setName("Acceleration Z");
    m_plotAcc->graph(2)->setData(m_vecX, m_vecAccZ);
    m_plotAcc->xAxis->setLabel("time");
    m_plotAcc->yAxis->setLabel("magnitude");

    m_plotAcc->rescaleAxes();
    m_plotAcc->replot();

    m_plotGyro->legend->setVisible(true);
    m_plotGyro->legend->setFont(legendFont);
    m_plotGyro->legend->setBrush(QBrush(QColor(255,255,255,230)));
    m_plotGyro->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    m_plotGyro->addGraph();
    m_plotGyro->graph(0)->setPen(QPen(QColor(Qt::blue)));
    m_plotGyro->graph(0)->setName("Angular speed X");
    m_plotGyro->graph(0)->setData(m_vecXGyro, m_vecGyroX);
    m_plotGyro->addGraph();
    m_plotGyro->graph(1)->setPen(QPen(QColor(Qt::red)));
    m_plotGyro->graph(1)->setName("Angular speed Y");
    m_plotGyro->graph(1)->setData(m_vecXGyro, m_vecGyroY);
    m_plotGyro->addGraph();
    m_plotGyro->graph(2)->setPen(QPen(QColor(Qt::green)));
    m_plotGyro->graph(2)->setName("Angular speed Z");
    m_plotGyro->graph(2)->setData(m_vecXGyro, m_vecGyroZ);
    m_plotGyro->xAxis->setLabel("time");
    m_plotGyro->yAxis->setLabel("magnitude");

    m_plotGyro->rescaleAxes();
    m_plotGyro->replot();

    m_pButtonSendStart = new QPushButton("Start");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_plotAcc);
    layout->addWidget(m_plotGyro);
    layout->addWidget(m_pButtonSendStart);
    setLayout(layout);

    connect(m_pTimerRedraw, SIGNAL(timeout()), this, SLOT(slotUpdateGraphView()));
    connect(m_pCommManager, SIGNAL(signalRenewAccel(int,int,int)), this, SLOT(slotUpdateAccVectors(int,int,int)));
    connect(m_pCommManager, SIGNAL(signalRenewGyro(int,int,int)), this, SLOT(slotUpdateGyroVectors(int,int,int)));
    connect(m_pButtonSendStart, SIGNAL(clicked()), this, SLOT(slotSendStart()));

    m_pCommManager->Connect();

    m_pTimerRedraw->start(62);
}

ClientView::~ClientView()
{
    delete m_pTimerRedraw;
    delete m_pCommManager;
}

void ClientView::slotSendStart()
{
    m_pCommManager->SendStart();
}

void ClientView::slotUpdateGraphView()
{
    m_plotAcc->graph(0)->setData(m_vecX, m_vecAccX);
    m_plotAcc->graph(1)->setData(m_vecX, m_vecAccY);
    m_plotAcc->graph(2)->setData(m_vecX, m_vecAccZ);
    m_plotAcc->replot();
    m_plotAcc->rescaleAxes();

    m_plotGyro->graph(0)->setData(m_vecXGyro, m_vecGyroX);
    m_plotGyro->graph(1)->setData(m_vecXGyro, m_vecGyroY);
    m_plotGyro->graph(2)->setData(m_vecXGyro, m_vecGyroZ);
    m_plotGyro->replot();
    m_plotGyro->rescaleAxes();
}

void ClientView::slotUpdateAccVectors(int nAccX, int nAccY, int nAccZ)
{
    m_vecX.append(m_fCurrentX);
    m_vecAccX.append(nAccX);
    m_vecAccY.append(nAccY);
    m_vecAccZ.append(nAccZ);
    if (m_vecX.size() > 500) {
        m_vecX.pop_front();
        m_vecAccX.pop_front();
        m_vecAccY.pop_front();
        m_vecAccZ.pop_front();
    }
    m_fCurrentX++;
}

void ClientView::slotUpdateGyroVectors(int nGyroX, int nGyroY, int nGyroZ)
{
    m_vecXGyro.append(m_fCurrentXGyro);
    m_vecGyroX.append(nGyroX);
    m_vecGyroY.append(nGyroY);
    m_vecGyroZ.append(nGyroZ);
    if (m_vecXGyro.size() > 500) {
        m_vecXGyro.pop_front();
        m_vecGyroX.pop_front();
        m_vecGyroY.pop_front();
        m_vecGyroZ.pop_front();
    }
    m_fCurrentXGyro++;
}
