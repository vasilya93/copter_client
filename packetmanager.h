#ifndef PACKETMANAGER_H
#define PACKETMANAGER_H

#include "datakeeper.h"
#include "fifo.h"
#include "stdint.h"

union Converter
{
  int16_t Short;
  float RealNum;
  int Integer;
  unsigned char Bytes[4];
};

class PacketManager
{
private:
    enum {
        MSNR_MT_BYTE = 1,
        MSNR_MT_WORD,
        MSNR_MT_DWRD
    };

    enum {
        MSNR_DD_NONE = 0x04,
        MSNR_DD_ACCELX = 0x08,
        MSNR_DD_ACCELY = 0x0C,
        MSNR_DD_ACCELZ = 0x10,
        MSNR_DD_WHOAMI = 0x14,
        MSNR_DD_PWRMGMT1 = 0x18,
        MSNR_DD_ANGSPEEDX = 0x1C,
        MSNR_DD_ANGSPEEDY = 0x20,
        MSNR_DD_ANGSPEEDZ = 0x24,
        MSNR_DD_GYROXOFF = 0x28,
        MSNR_DD_GYROYOFF = 0x2C,
        MSNR_DD_GYROZOFF = 0x30,
        MSNR_DD_ACCELXOFF = 0x34,
        MSNR_DD_ACCELYOFF = 0x38,
        MSNR_DD_ACCELZOFF = 0x3C,
        MSNR_DD_DCM11 = 0x40,
        MSNR_DD_DCM12 = 0x44,
        MSNR_DD_DCM13 = 0x48,
        MSNR_DD_DCM21 = 0x4C,
        MSNR_DD_DCM22 = 0x50,
        MSNR_DD_DCM23 = 0x54,
        MSNR_DD_DCM31 = 0x58,
        MSNR_DD_DCM32 = 0x5C,
        MSNR_DD_DCM33 = 0x60,
        MSNR_DD_HMC5883_IDA = 0x64,
        MSNR_DD_HMC5883_IDB = 0x68,
        MSNR_DD_HMC5883_IDC = 0x6C,
        MSNR_DD_SENSFUS_ACCXNORM = 0x70,
        MSNR_DD_SENSFUS_ACCYNORM = 0x74,
        MSNR_DD_SENSFUS_ACCZNORM = 0x78,
        MSNR_DD_SENSFUS_ROLL = 0x7C,
        MSNR_DD_SENSFUS_PITCH = 0x80,
        MSNR_DD_SENSFUS_YAW = 0x84
    };

    enum {
        I2C_MSG_STRTST = 1,
        I2C_MSG_SBSND,
        I2C_MSG_ADSND,
        I2C_MSG_TRNSMTR,
        I2C_MSG_NACKST,
        I2C_MSG_RCVR,
        I2C_MSG_TRSCPL,
        I2C_MSG_TXE,
        I2C_MSG_RXNE,
        I2C_MSG_RXNE0,
        I2C_MSG_RXNE1,
        I2C_MSG_AF,
        I2C_MSG_STRTFL,
        I2C_MSG_ERR,

        MS5611_MSG_NODATA = 31,
        MS5611_MSG_SENST1REQ,
        MS5611_MSG_SENST1,
        MS5611_MSG_OFFT1REQ,
        MS5611_MSG_OFFT1,

        MPU6050_MSG_WHOAMI = 36,
        MPU6050_MSG_GETNEXTINIT,
        MPU6050_MSG_PROCOPRES,
        MPU6050_MSG_GETNEXTREG,
        MPU6050_MSG_ACCELX,
        MPU6050_MSG_ACCELY,
        MPU6050_MSG_ACCELZ,
        MPU6050_MSG_PWR1,
        MPU6050_MSG_PWR1_BAD,
        MPU6050_MSG_PWR1_GOOD,
        MPU6050_MSG_INIT_COMP,
        MPU6050_MSG_GYROXGIVEN,
        MPU6050_MSG_GYROXPROC,
        MPU6050_MSG_CYCLECOMP,

        SENSFUS_MSG_ACC_INSIDE = 51,
        SENSFUS_MSG_GYRO_INSIDE,

        HMC5884_MSG_IDA = 61,
        HMC5884_MSG_IDB,
        HMC5884_MSG_IDC,
        HMC5884_MSG_INIT_COMP
    };

public:
    PacketManager();
    ~PacketManager();

    int Initialize();
    void PushReceived(const unsigned char* pArray, unsigned int nSize);

    inline void SetDataKeeper(DataKeeper *pDataKeeper)
    {
        m_pDataKeeper = pDataKeeper;
    }

private:
    void ParsePacket(const unsigned char* pPacket);
    void ProcessDataDescription(unsigned char nHeader);

    DataKeeper *m_pDataKeeper;
    CFifo<unsigned char> m_fifoInput;
    Converter m_converter;

    unsigned int SIZE_PACKET = 3;
    unsigned int SIZE_BUFFER = 2048;
    unsigned int MSNR_DD_MASK = 0xFC;
};

#endif // PACKETMANAGER_H
