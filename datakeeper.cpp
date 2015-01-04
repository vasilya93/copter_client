#include "datakeeper.h"

DataKeeper::DataKeeper() :
    m_nAccXLast(0),
    m_nAccYLast(0),
    m_nAccZLast(0),
    m_nGyroXLast(0),
    m_nGyroYLast(0),
    m_nGyroZLast(0),
    m_nRenewStatus(0)
{
}

DataKeeper::~DataKeeper()
{
}

void DataKeeper::SetGyroX(int nGyroX)
{
    m_vecGyroX.push_back(nGyroX);
    m_nGyroXLast = nGyroX;
    m_nRenewStatus |= GYROX_RENEWED;
}

void DataKeeper::SetGyroY(int nGyroY)
{
    m_vecGyroY.push_back(nGyroY);
    m_nGyroYLast = nGyroY;
    m_nRenewStatus |= GYROY_RENEWED;
}

void DataKeeper::SetGyroZ(int nGyroZ)
{
    m_vecGyroZ.push_back(nGyroZ);
    m_nGyroZLast = nGyroZ;
    m_nRenewStatus |= GYROZ_RENEWED;
}

void DataKeeper::SetAccX(int nAccX)
{
    m_vecAccX.push_back(nAccX);
    m_nAccXLast = nAccX;
    m_nRenewStatus |= ACCX_RENEWED;
}

void DataKeeper::SetAccY(int nAccY)
{
    m_vecAccY.push_back(nAccY);
    m_nAccYLast = nAccY;
    m_nRenewStatus |= ACCY_RENEWED;
}

void DataKeeper::SetAccZ(int nAccZ)
{
    m_vecAccZ.push_back(nAccZ);
    m_nAccZLast = nAccZ;
    m_nRenewStatus |= ACCZ_RENEWED;
}

void DataKeeper::GetAcc(int &nAccX, int &nAccY, int &nAccZ)
{
    nAccX = m_nAccXLast;
    nAccY = m_nAccYLast;
    nAccZ = m_nAccZLast;

    m_nRenewStatus &= ~(ACCX_RENEWED | ACCY_RENEWED | ACCZ_RENEWED);
}

void DataKeeper::GetGyro(int &nGyroX, int &nGyroY, int &nGyroZ)
{
    nGyroX = m_nGyroXLast;
    nGyroY = m_nGyroYLast;
    nGyroZ = m_nGyroZLast;

    m_nRenewStatus &= ~(GYROX_RENEWED | GYROY_RENEWED | GYROZ_RENEWED);
}

bool DataKeeper::IsAccReady()
{
    return ((m_nRenewStatus & ACCX_RENEWED) &&
            (m_nRenewStatus & ACCY_RENEWED) &&
            (m_nRenewStatus & ACCZ_RENEWED));
}

bool DataKeeper::IsGyroReady()
{
    return ((m_nRenewStatus & GYROX_RENEWED) &&
            (m_nRenewStatus & GYROY_RENEWED) &&
            (m_nRenewStatus & GYROZ_RENEWED));
}
