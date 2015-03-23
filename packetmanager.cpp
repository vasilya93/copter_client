#include "packetmanager.h"

PacketManager::PacketManager()
{
}

PacketManager::~PacketManager()
{
}

int PacketManager::Initialize()
{
    return m_fifoInput.Initialize(SIZE_BUFFER);
}

void PacketManager::PushReceived(const unsigned char *pArray, unsigned int nSize)
{
    m_fifoInput.PushArray(pArray, nSize);
    while (m_fifoInput.OccupiedElems() >= SIZE_PACKET) {
        unsigned char *pCurrentPacket = m_fifoInput.PopArray(SIZE_PACKET);
        ParsePacket(pCurrentPacket);
        delete pCurrentPacket;
    }
}

void PacketManager::ParsePacket(const unsigned char* pPacket)
{
    switch (pPacket[0] & ~MSNR_DD_MASK) {
    case MSNR_MT_BYTE:
        break;
    case MSNR_MT_WORD:
        m_converter.Bytes[0] = pPacket[2];
        m_converter.Bytes[1] = pPacket[1];
        m_converter.Bytes[2] = 0;
        m_converter.Bytes[3] = 0;
        ProcessDataDescription(pPacket[0]);
        break;
    case MSNR_MT_DWRD:
        m_converter.Bytes[0] = pPacket[2/*4*/];
        m_converter.Bytes[1] = pPacket[1/*3*/];
        m_converter.Bytes[2] = pPacket[2];
        m_converter.Bytes[3] = pPacket[1];
        ProcessDataDescription(pPacket[0]);
        break;
    default:
        qDebug("ProcessPackageType: error, unknown package type!\n");
        break;
    }
}

void PacketManager::ProcessDataDescription(unsigned char nHeader)
{
    nHeader &= MSNR_DD_MASK;
    switch (nHeader) {
    case MSNR_DD_ACCELX:
        m_pDataKeeper->SetAccX(m_converter.Short);
        break;
    case MSNR_DD_ACCELY:
        m_pDataKeeper->SetAccY(m_converter.Short);
        break;
    case MSNR_DD_ACCELZ:
        m_pDataKeeper->SetAccZ(m_converter.Short);
        break;
    case MSNR_DD_ANGSPEEDX:
        m_pDataKeeper->SetGyroX(m_converter.Short);
        break;
    case MSNR_DD_ANGSPEEDY:
        m_pDataKeeper->SetGyroY(m_converter.Short);
        break;
    case MSNR_DD_ANGSPEEDZ:
        m_pDataKeeper->SetGyroZ(m_converter.Short);
        break;
    case MSNR_DD_GYROXOFF:
        //m_pDataKeeper->SetGyroX(m_converter.Short);
        break;
    case MSNR_DD_GYROYOFF:
        //m_pDataKeeper->SetGyroY(m_converter.Short);
        //m_pDataKeeper->SetGyroZ(m_converter.Short);
        break;
    case MSNR_DD_ACCELXOFF:
        //m_pDataKeeper->SetAccX(m_converter.Short);
        //m_pDataKeeper->SetAccY(m_converter.Short);
        //m_pDataKeeper->SetAccZ(m_converter.Short);
        break;
    case MSNR_DD_SENSFUS_ROLL:
        m_pDataKeeper->SetRoll(m_converter.RealNum);
        break;
    case MSNR_DD_SENSFUS_PITCH:
        m_pDataKeeper->SetPitch(m_converter.RealNum);
        break;
    case MSNR_DD_SENSFUS_YAW:
        m_pDataKeeper->SetYaw(m_converter.RealNum);
        break;
    /*case MSNR_DD_DCM11:
        _dataKeeper.setDCM11(_converter.RealNum);
        break;
    case MSNR_DD_DCM12:
        _dataKeeper.setDCM12(_converter.RealNum);
        break;
    case MSNR_DD_DCM13:
        _dataKeeper.setDCM13(_converter.RealNum);
        break;
    case MSNR_DD_DCM21:
        _dataKeeper.setDCM21(_converter.RealNum);
        break;
    case MSNR_DD_DCM22:
        _dataKeeper.setDCM22(_converter.RealNum);
        break;
    case MSNR_DD_DCM23:
        _dataKeeper.setDCM23(_converter.RealNum);
        break;
    case MSNR_DD_DCM31:
        _dataKeeper.setDCM31(_converter.RealNum);
        break;
    case MSNR_DD_DCM32:
        _dataKeeper.setDCM32(_converter.RealNum);
        break;
    case MSNR_DD_DCM33:
        _dataKeeper.setDCM33(_converter.RealNum);
        break;
    case MSNR_DD_GYROXOFF:
        printf("gyro x offset value: %hd\n", (int16_t) _converter.Integer);
        break;
    case MSNR_DD_GYROYOFF:
        printf("gyro y offset value: %hd\n", (int16_t) _converter.Integer);
        break;
    case MSNR_DD_GYROZOFF:
        printf("gyro z offset value: %hd\n", (int16_t) _converter.Integer);
        break;
    case MSNR_DD_ACCELXOFF:
        printf("accel x offset value: %hd\n", (int16_t) _converter.Integer);
        break;
    case MSNR_DD_ACCELYOFF:
        printf("accel y offset value: %hd\n", (int16_t) _converter.Integer);
        break;
    case MSNR_DD_ACCELZOFF:
        printf("accel z offset value: %hd\n", (int16_t) _converter.Integer);
        break;
    case MSNR_DD_WHOAMI:
        printf("who am i value: %u\n", _converter.Integer);
        break;
    case MSNR_DD_PWRMGMT1:
        printf("PWRMGMT1 value: %u\n", _converter.Integer);
        break;
    case MSNR_DD_HMC5883_IDA:
        printf("HMC5883 IDA value: %u\n", _converter.Integer);
        break;
    case MSNR_DD_HMC5883_IDB:
        printf("HMC5883 IDB value: %u\n", _converter.Integer);
        break;
    case MSNR_DD_HMC5883_IDC:
        printf("HMC5883 IDC value: %u\n", _converter.Integer);
        break;
    case MSNR_DD_SENSFUS_ACCXNORM:
        printf("Accel normalized value on x: %u\n", _converter.Integer);
        break;
    case MSNR_DD_SENSFUS_ACCYNORM:
        printf("Accel normalized value on y: %u\n", _converter.Integer);
        break;
    case MSNR_DD_SENSFUS_ACCZNORM:
        printf("Accel normalized value on z: %u\n", _converter.Integer);
        break;*/
    default:
        /*printf("ProcessDataDescription: error, unknown data description: %u\n",
               messageHeader & MSNR_DD_MASK);
        printf("ProcessDataDescription: data: %u\n", _converter.Integer);*/
        qDebug("ProcessDataDescription: error, unknown data description!\n");
        break;
    }
}
