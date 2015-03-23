#include <QVBoxLayout>
#include <QHBoxLayout>
#include <math.h>
#include "ClientView.h"

ClientView::ClientView(QWidget *parent)
    : QWidget(parent),
      m_fCurrentXAccX(0),
      m_fCurrentXAccY(0),
      m_fCurrentXAccZ(0),
      m_fCurrentXGyroX(0),
      m_fCurrentXGyroY(0),
      m_fCurrentXGyroZ(0),
      m_bDoAllowEdit(true)
{
    m_plotAcc = new QCustomPlot;
    m_plotGyro = new QCustomPlot;
    m_pTimerRedraw = new QTimer;
    m_pCommManager = new CommManager;

    m_vecXAccX.append(0);
    m_vecXAccY.append(0);
    m_vecXAccZ.append(0);
    m_vecXGyroX.append(0);
    m_vecXGyroY.append(0);
    m_vecXGyroZ.append(0);

    m_vecAccX.append(0);
    m_vecAccY.append(0);
    m_vecAccZ.append(0);
    m_vecGyroX.append(0);
    m_vecGyroY.append(0);
    m_vecGyroZ.append(0);

    m_plotAcc->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(9); // and make a bit smaller for legend
    m_plotAcc->legend->setFont(legendFont);
    m_plotAcc->legend->setBrush(QBrush(QColor(255,255,255,230)));
    m_plotAcc->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignBottom|Qt::AlignRight);

    m_plotAcc->addGraph();
    m_plotAcc->graph(0)->setPen(QPen(QColor(Qt::blue)));
    m_plotAcc->graph(0)->setName("Acceleration X");
    m_plotAcc->graph(0)->setData(m_vecXAccX, m_vecAccX);
    m_plotAcc->addGraph();
    m_plotAcc->graph(1)->setPen(QPen(QColor(Qt::red)));
    m_plotAcc->graph(1)->setName("Acceleration Y");
    m_plotAcc->graph(1)->setData(m_vecXAccY, m_vecAccY);
    m_plotAcc->addGraph();
    m_plotAcc->graph(2)->setPen(QPen(QColor(Qt::green)));
    m_plotAcc->graph(2)->setName("Acceleration Z");
    m_plotAcc->graph(2)->setData(m_vecXAccZ, m_vecAccZ);
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
    m_plotGyro->graph(0)->setData(m_vecXGyroX, m_vecGyroX);
    m_plotGyro->addGraph();
    m_plotGyro->graph(1)->setPen(QPen(QColor(Qt::red)));
    m_plotGyro->graph(1)->setName("Angular speed Y");
    m_plotGyro->graph(1)->setData(m_vecXGyroY, m_vecGyroY);
    m_plotGyro->addGraph();
    m_plotGyro->graph(2)->setPen(QPen(QColor(Qt::green)));
    m_plotGyro->graph(2)->setName("Angular speed Z");
    m_plotGyro->graph(2)->setData(m_vecXGyroZ, m_vecGyroZ);
    m_plotGyro->xAxis->setLabel("time");
    m_plotGyro->yAxis->setLabel("magnitude");

    m_plotGyro->rescaleAxes();
    m_plotGyro->replot();

    m_pButtonSendStart = new QPushButton("Start");

    m_pLabelTimeGap = new QLabel("No timegap");
    m_pLabelTimeGap->setFixedHeight(20);

    m_editDCM = new QTextEdit;
    m_editDCM->setFixedHeight(70);
    m_editDCM->setReadOnly(true);

    m_pIndicatorRoll = new OrientationIndicator;
    m_pIndicatorPitch = new OrientationIndicator;
    m_pIndicatorYaw = new OrientationIndicator;

    QVBoxLayout *layoutFirstColumn = new QVBoxLayout;
    layoutFirstColumn->addWidget(m_plotAcc);
    layoutFirstColumn->addWidget(m_plotGyro);
    layoutFirstColumn->addWidget(m_pButtonSendStart);
    layoutFirstColumn->addWidget(m_pLabelTimeGap);

    QVBoxLayout *layoutSecondColumn = new QVBoxLayout;
    layoutSecondColumn->addWidget(m_pIndicatorRoll);
    layoutSecondColumn->addWidget(m_pIndicatorPitch);
    layoutSecondColumn->addWidget(m_pIndicatorYaw);
    layoutSecondColumn->addWidget(m_editDCM);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addLayout(layoutFirstColumn, 3);
    layout->addLayout(layoutSecondColumn, 1);
    setLayout(layout);


    connect(m_pTimerRedraw, SIGNAL(timeout()), this, SLOT(slotUpdateGraphView()));
    connect(m_pButtonSendStart, SIGNAL(clicked()), this, SLOT(slotSendStart()));

    m_pCommManager->Connect();

    m_pTimerRedraw->start(50/*62*/);
}

ClientView::~ClientView()
{
    delete m_pTimerRedraw;
    delete m_pCommManager;
    delete m_pIndicatorRoll;
    delete m_pIndicatorPitch;
    delete m_pIndicatorYaw;
    delete m_pLabelTimeGap;
}

void ClientView::slotSendStart()
{
    m_pCommManager->SendStart();
}

void ClientView::slotUpdateGraphView()
{
    QVector<double> vecAccX, vecAccY, vecAccZ,
                    vecGyroX, vecGyroY, vecGyroZ;

    m_pCommManager->GetAccUnread(vecAccX, vecAccY, vecAccZ);
    m_pCommManager->GetGyroUnread(vecGyroX, vecGyroY, vecGyroZ);

    AddValuesToAccVector(vecAccX, vecAccY, vecAccZ);
    AddValuesToGyroVector(vecGyroX, vecGyroY, vecGyroZ);

    m_plotAcc->graph(0)->setData(m_vecXAccX, m_vecAccX);
    m_plotAcc->graph(1)->setData(m_vecXAccY, m_vecAccY);
    m_plotAcc->graph(2)->setData(m_vecXAccZ, m_vecAccZ);
    m_plotAcc->replot();
    m_plotAcc->rescaleAxes();

    m_plotGyro->graph(0)->setData(m_vecXGyroX, m_vecGyroX);
    m_plotGyro->graph(1)->setData(m_vecXGyroY, m_vecGyroY);
    m_plotGyro->graph(2)->setData(m_vecXGyroZ, m_vecGyroZ);
    m_plotGyro->replot();
    m_plotGyro->rescaleAxes();
}

void ClientView::AddValuesToAccVector(int nAccX, int nAccY, int nAccZ)
{
        m_vecXAccX.append(m_fCurrentXAccX);
        m_vecXAccY.append(m_fCurrentXAccY);
        m_vecXAccZ.append(m_fCurrentXAccZ);
        m_vecAccX.append(nAccX);
        m_vecAccY.append(nAccY);
        m_vecAccZ.append(nAccZ);
        if (m_vecXAccX.size() > SIZE_SET) {
            m_vecXAccX.pop_front();
            m_vecAccX.pop_front();
        }

        if (m_vecXAccY.size() > SIZE_SET) {
            m_vecXAccY.pop_front();
            m_vecAccY.pop_front();
        }

        if (m_vecXAccZ.size() > SIZE_SET) {
            m_vecXAccZ.pop_front();
            m_vecAccZ.pop_front();
        }

        m_fCurrentXAccX++;
        m_fCurrentXAccY++;
        m_fCurrentXAccZ++;
}

void ClientView::AddValuesToGyroVector(int nGyroX, int nGyroY, int nGyroZ)
{
        m_vecXGyroX.append(m_fCurrentXGyroX);
        m_vecXGyroY.append(m_fCurrentXGyroY);
        m_vecXGyroZ.append(m_fCurrentXGyroZ);
        m_vecGyroX.append(nGyroX);
        m_vecGyroY.append(nGyroY);
        m_vecGyroZ.append(nGyroZ);

        if (m_vecXGyroX.size() > SIZE_SET) {
            m_vecXGyroX.pop_front();
            m_vecGyroX.pop_front();
        }

        if (m_vecXGyroY.size() > SIZE_SET) {
            m_vecXGyroY.pop_front();
            m_vecGyroY.pop_front();
        }

        if (m_vecXGyroZ.size() > SIZE_SET) {
            m_vecXGyroZ.pop_front();
            m_vecGyroZ.pop_front();
        }
        m_fCurrentXGyroX++;
        m_fCurrentXGyroY++;
        m_fCurrentXGyroZ++;
}

void ClientView::AddValuesToAccVector(QVector<double> vecAccX,
                                      QVector<double> vecAccY,
                                      QVector<double> vecAccZ)
{
    m_vecAccX += vecAccX;
    m_vecAccY += vecAccY;
    m_vecAccZ += vecAccZ;

    while (m_fCurrentXAccX++ < m_vecAccX.size()) {
        m_vecXAccX.append(m_fCurrentXAccX);
    }

    while (m_fCurrentXAccY++ < m_vecAccY.size()) {
        m_vecXAccY.append(m_fCurrentXAccY);
    }

    while (m_fCurrentXAccZ++ < m_vecAccZ.size()) {
        m_vecXAccZ.append(m_fCurrentXAccZ);
    }

//    if (m_vecXAccX.size() > SIZE_SET) {
//        m_vecXAccX.erase(m_vecXAccX.begin(), m_vecXAccX.begin() + (m_vecXAccX.size() - SIZE_SET));
//        m_vecAccX.erase(m_vecAccX.begin(), m_vecAccX.begin() + (m_vecAccX.size() - SIZE_SET));
//    }

//    if (m_vecXAccY.size() > SIZE_SET) {
//        m_vecXAccY.erase(m_vecXAccY.begin(), m_vecXAccY.begin() + (m_vecXAccY.size() - SIZE_SET));
//        m_vecAccY.erase(m_vecAccY.begin(), m_vecAccY.begin() + (m_vecAccY.size() - SIZE_SET));
//    }

//    if (m_vecXAccZ.size() > SIZE_SET) {
//        m_vecXAccZ.erase(m_vecXAccZ.begin(), m_vecXAccZ.begin() + (m_vecXAccZ.size() - SIZE_SET));
//        m_vecAccZ.erase(m_vecAccZ.begin(), m_vecAccZ.begin() + (m_vecAccZ.size() - SIZE_SET));
//    }
}

void ClientView::AddValuesToGyroVector(QVector<double> vecGyroX,
                                       QVector<double> vecGyroY,
                                       QVector<double> vecGyroZ)
{
    m_vecGyroX += vecGyroX;
    m_vecGyroY += vecGyroY;
    m_vecGyroZ += vecGyroZ;

    while (m_fCurrentXGyroX++ < m_vecGyroX.size()) {
        m_vecXGyroX.append(m_fCurrentXGyroX);
    }

    while (m_fCurrentXGyroY++ < m_vecGyroY.size()) {
        m_vecXGyroY.append(m_fCurrentXGyroY);
    }

    while (m_fCurrentXGyroZ++ < m_vecGyroZ.size()) {
        m_vecXGyroZ.append(m_fCurrentXGyroZ);
    }

//    if (m_vecXGyroX.size() > SIZE_SET) {
//        m_vecXGyroX.erase(m_vecXGyroX.begin(), m_vecXGyroX.begin() + (m_vecXGyroX.size() - SIZE_SET));
//        m_vecGyroX.erase(m_vecGyroX.begin(), m_vecGyroX.begin() + (m_vecGyroX.size() - SIZE_SET));
//    }

//    if (m_vecXGyroY.size() > SIZE_SET) {
//        m_vecXGyroY.erase(m_vecXGyroY.begin(), m_vecXGyroY.begin() + (m_vecXGyroY.size() - SIZE_SET));
//        m_vecGyroY.erase(m_vecGyroY.begin(), m_vecGyroY.begin() + (m_vecGyroY.size() - SIZE_SET));
//    }

//    if (m_vecXGyroZ.size() > SIZE_SET) {
//        m_vecXGyroZ.erase(m_vecXGyroZ.begin(), m_vecXGyroZ.begin() + (m_vecXGyroZ.size() - SIZE_SET));
//        m_vecGyroZ.erase(m_vecGyroZ.begin(), m_vecGyroZ.begin() + (m_vecGyroZ.size() - SIZE_SET));
//    }
}
