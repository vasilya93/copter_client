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

    if (m_pDataKeeper->IsDCMReady()) {
        vec3 vec3ITiedCurrent, vec3JTiedCurrent, vec3KTiedCurrent;
        float fRoll, fPitch, fYaw;
        m_pDataKeeper->GetDCM(vec3ITiedCurrent, vec3JTiedCurrent, vec3KTiedCurrent);
        m_pDataKeeper->GetAngles(fRoll, fPitch, fYaw);
        emit signalRenewDCM(vec3ITiedCurrent.x, vec3ITiedCurrent.y, vec3ITiedCurrent.z,
                            vec3JTiedCurrent.x, vec3JTiedCurrent.y, vec3JTiedCurrent.z,
                            vec3KTiedCurrent.x, vec3KTiedCurrent.y, vec3KTiedCurrent.z,
                            fRoll, fPitch, fYaw);
    }
}
