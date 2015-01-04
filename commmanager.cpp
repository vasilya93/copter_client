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

void CommManager::SerialBytesReceivedHandler(unsigned char *pBytesReceived, unsigned int nSize)
{
    m_pPacketManager->PushReceived(pBytesReceived, nSize);

    if (m_pDataKeeper->IsAccReady()) {
        int nAccX, nAccY, nAccZ;
        m_pDataKeeper->GetAcc(nAccX, nAccY, nAccZ);
        emit signalRenewAccel(nAccX, nAccY, nAccZ);
    }

    if (m_pDataKeeper->IsGyroReady()) {
        int nGyroX, nGyroY, nGyroZ;
        m_pDataKeeper->GetGyro(nGyroX, nGyroY, nGyroZ);
        emit signalRenewGyro(nGyroX, nGyroY, nGyroZ);
    }
}
