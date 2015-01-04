#include <QVBoxLayout>
#include <math.h>
#include "ClientView.h"

ClientView::ClientView(QWidget *parent)
    : QWidget(parent),
      m_fCurrentX(0)
{
    m_plot = new QCustomPlot;
    m_pTimerRedraw = new QTimer;
    m_pCommManager = new CommManager;

    m_vecX.append(m_fCurrentX);
    m_vecY1.append(0);
    m_vecY2.append(0);
    m_vecY3.append(0);
    m_fCurrentX += 1;

    m_plot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    m_plot->legend->setFont(legendFont);
    m_plot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    m_plot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    m_plot->addGraph();
    m_plot->graph(0)->setPen(QPen(QColor(Qt::blue)));
    m_plot->graph(0)->setName("Acceleration X");
    m_plot->graph(0)->setData(m_vecX, m_vecY1);
    m_plot->addGraph();
    m_plot->graph(1)->setPen(QPen(QColor(Qt::red)));
    m_plot->graph(1)->setName("Acceleration Y");
    m_plot->graph(1)->setData(m_vecX, m_vecY2);
    m_plot->addGraph();
    m_plot->graph(2)->setPen(QPen(QColor(Qt::green)));
    m_plot->graph(2)->setName("Acceleration Z");
    m_plot->graph(2)->setData(m_vecX, m_vecY3);
    m_plot->xAxis->setLabel("time");
    m_plot->yAxis->setLabel("magnitude");

    m_plot->rescaleAxes();
    m_plot->replot();

    m_pButtonSendStart = new QPushButton("Start");

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_plot);
    layout->addWidget(m_pButtonSendStart);
    setLayout(layout);

    connect(m_pTimerRedraw, SIGNAL(timeout()), this, SLOT(slotUpdateGraphView()));
    connect(m_pCommManager, SIGNAL(signalRenewAccel(int,int,int)), this, SLOT(slotUpdateAccVectors(int,int,int)));
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
    m_plot->graph(0)->setData(m_vecX, m_vecY1);
    m_plot->graph(1)->setData(m_vecX, m_vecY2);
    m_plot->graph(2)->setData(m_vecX, m_vecY3);
    m_plot->replot();
    m_plot->rescaleAxes();
}

void ClientView::slotUpdateAccVectors(int nAccX, int nAccY, int nAccZ)
{
    m_vecX.append(m_fCurrentX);
    m_vecY1.append(nAccX);
    m_vecY2.append(nAccY);
    m_vecY3.append(nAccZ);
    if (m_vecX.size() > 200) {
        m_vecX.pop_front();
        m_vecY1.pop_front();
        m_vecY2.pop_front();
        m_vecY3.pop_front();
    }
    m_fCurrentX++;
}

void ClientView::slotUpdateGyroVectors(int nGyroX, int nGyroY, int nGyroZ)
{
}
