#include "commmanager.h"

CommManager::CommManager(QObject *parent) :
    QObject(parent),
    SerialCommSubscribable()
{
    m_pSerialComm = new SerialComm;
    m_pSerialComm->AttachHandlerHost(this);

    m_pDataKeeper = new DataKeeper;

    m_pPacketManager = new PacketManager;
    m_pPacketManager->Initialize();
    m_pPacketManager->SetDataKeeper(m_pDataKeeper);
}

CommManager::~CommManager()
{
    delete m_pSerialComm;
    delete m_pPacketManager;
    delete m_pDataKeeper;
}

void CommManager::GetAccUnread(QVector<double> &vecAccX, QVector<double> &vecAccY, QVector<double> &vecAccZ)
{
    vecAccX = QVector<double>::fromStdVector(m_pDataKeeper->GetAccXUnread());
    m_pDataKeeper->ClearAccXUnread();
    vecAccY = QVector<double>::fromStdVector(m_pDataKeeper->GetAccYUnread());
    m_pDataKeeper->ClearAccYUnread();
    vecAccZ = QVector<double>::fromStdVector(m_pDataKeeper->GetAccZUnread());
    m_pDataKeeper->ClearAccZUnread();
}

void CommManager::GetGyroUnread(QVector<double> &vecGyroX, QVector<double> &vecGyroY, QVector<double> &vecGyroZ)
{
    vecGyroX = QVector<double>::fromStdVector(m_pDataKeeper->GetGyroXUnread());
    m_pDataKeeper->ClearGyroXUnread();
    vecGyroY = QVector<double>::fromStdVector(m_pDataKeeper->GetGyroYUnread());
    m_pDataKeeper->ClearGyroYUnread();
    vecGyroZ = QVector<double>::fromStdVector(m_pDataKeeper->GetGyroZUnread());
    m_pDataKeeper->ClearGyroZUnread();
}

void CommManager::SerialBytesReceivedHandler(unsigned char *pBytesReceived, unsigned int nSize)
{
    m_pPacketManager->PushReceived(pBytesReceived, nSize);
}
