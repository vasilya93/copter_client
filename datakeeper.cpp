#include "datakeeper.h"

DataKeeper::DataKeeper() :
    m_nAccXLast(0),
    m_nAccYLast(0),
    m_nAccZLast(0),
    m_nGyroXLast(0),
    m_nGyroYLast(0),
    m_nGyroZLast(0),
    m_nRenewStatus(0),
    m_nAccXCalibrationAccum(0),
    m_nAccYCalibrationAccum(0),
    m_nAccZCalibrationAccum(0),
    m_nGyroXCalibrationAccum(0),
    m_nGyroYCalibrationAccum(0),
    m_nGyroZCalibrationAccum(0),
    m_nAccXCalibrationCounter(0),
    m_nAccYCalibrationCounter(0),
    m_nAccZCalibrationCounter(0),
    m_nGyroXCalibrationCounter(0),
    m_nGyroYCalibrationCounter(0),
    m_nGyroZCalibrationCounter(0),
    m_nAccXOffset(0),
    m_nAccYOffset(0),
    m_nAccZOffset(0),
    m_nGyroXOffset(0),
    m_nGyroYOffset(0),
    m_nGyroZOffset(0)
{
}

DataKeeper::~DataKeeper()
{
}

void DataKeeper::SetGyroX(int nGyroX)
{
    if (m_nGyroXCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nGyroXCalibrationAccum += nGyroX;
        if (++m_nGyroXCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nGyroXOffset = m_nGyroXCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    m_nGyroXLast = nGyroX - m_nGyroXOffset;
    m_vecGyroX.push_back(m_nGyroXLast);
    m_nRenewStatus |= GYROX_RENEWED;
}

void DataKeeper::SetGyroY(int nGyroY)
{
    if (m_nGyroYCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nGyroYCalibrationAccum += nGyroY;
        if (++m_nGyroYCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nGyroYOffset = m_nGyroYCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    m_nGyroYLast = nGyroY - m_nGyroYOffset;
    m_vecGyroY.push_back(m_nGyroYLast);
    m_nRenewStatus |= GYROY_RENEWED;
}

void DataKeeper::SetGyroZ(int nGyroZ)
{
    if (m_nGyroZCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nGyroZCalibrationAccum += nGyroZ;
        if (++m_nGyroZCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nGyroZOffset = m_nGyroZCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    m_nGyroZLast = nGyroZ - m_nGyroZOffset;
    m_vecGyroZ.push_back(m_nGyroZLast);
    m_nRenewStatus |= GYROZ_RENEWED;
}

void DataKeeper::SetAccX(int nAccX)
{
    if (m_nAccXCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nAccXCalibrationAccum += nAccX;
        if (++m_nAccXCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nAccXOffset = m_nAccXCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    m_nAccXLast = nAccX - m_nAccXOffset;
    m_vecAccX.push_back(m_nAccXLast);
    m_nRenewStatus |= ACCX_RENEWED;
}

void DataKeeper::SetAccY(int nAccY)
{
    if (m_nAccYCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nAccYCalibrationAccum += nAccY;
        if (++m_nAccYCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nAccYOffset = m_nAccYCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    m_nAccYLast = nAccY - m_nAccYOffset;
    m_vecAccY.push_back(m_nAccYLast);
    m_nRenewStatus |= ACCY_RENEWED;
}

void DataKeeper::SetAccZ(int nAccZ)
{
    if (m_nAccZCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nAccZCalibrationAccum += nAccZ;
        if (++m_nAccZCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nAccZOffset = m_nAccZCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    m_nAccZLast = nAccZ - m_nAccZOffset;
    m_vecAccZ.push_back(m_nAccZLast);
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
