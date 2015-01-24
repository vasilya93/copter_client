#include <stdlib.h>
#include <math.h>
#include "datakeeper.h"

int compare(const void *a, const void *b);

DataKeeper::DataKeeper() :
    m_nAccXLast(0),
    m_nAccYLast(0),
    m_nAccZLast(0),
    m_nGyroXLast(0),
    m_nGyroYLast(0),
    m_nGyroZLast(0),
    m_nRenewStatus(0),
    m_nReadyStatus(0),
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
    m_fifoAccXFiltration.Initialize(SIZE_AVERAGING_WINDOW);
    m_fifoAccYFiltration.Initialize(SIZE_AVERAGING_WINDOW);
    m_fifoAccZFiltration.Initialize(SIZE_AVERAGING_WINDOW);
    m_fifoGyroXFiltration.Initialize(SIZE_AVERAGING_WINDOW);
    m_fifoGyroYFiltration.Initialize(SIZE_AVERAGING_WINDOW);
    m_fifoGyroZFiltration.Initialize(SIZE_AVERAGING_WINDOW);

    switch (FILTRATION_TYPE) {
    case FILTRATION_AVERAGE:
        GetWindowWeightedValue = &DataKeeper::GetWindowAverage;
        break;
    case FILTRATION_MEDIAN:
        GetWindowWeightedValue = &DataKeeper::GetWindowMedian;
        break;
    }
}

DataKeeper::~DataKeeper()
{
}

void DataKeeper::SetGyroX(int nGyroX)
{
    if (m_nGyroXCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nGyroXCalibrationAccum += nGyroX;
        if (++m_nGyroXCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nGyroXOffset = - m_nGyroXCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    int nAlignedGyroX = nGyroX + m_nGyroXOffset;
    m_nGyroXLast = FilterValue(nAlignedGyroX, &m_fifoGyroXFiltration);
    m_vecGyroX.push_back(m_nGyroXLast);
    m_nRenewStatus |= GYROX_RENEWED;
}

void DataKeeper::SetGyroY(int nGyroY)
{
    if (m_nGyroYCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nGyroYCalibrationAccum += nGyroY;
        if (++m_nGyroYCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nGyroYOffset = - m_nGyroYCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    int nAlignedGyroY = nGyroY + m_nGyroYOffset;
    m_nGyroYLast = FilterValue(nAlignedGyroY, &m_fifoGyroYFiltration);
    m_vecGyroY.push_back(m_nGyroYLast);
    m_nRenewStatus |= GYROY_RENEWED;
}

void DataKeeper::SetGyroZ(int nGyroZ)
{
    if (m_nGyroZCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nGyroZCalibrationAccum += nGyroZ;
        if (++m_nGyroZCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nGyroZOffset = - m_nGyroZCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    int nAlignedGyroZ = nGyroZ + m_nGyroZOffset;
    m_nGyroZLast = FilterValue(nAlignedGyroZ, &m_fifoGyroZFiltration);
    m_vecGyroZ.push_back(m_nGyroZLast);
    m_nRenewStatus |= GYROZ_RENEWED;
}

void DataKeeper::SetAccX(int nAccX)
{
    if (m_nAccXCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nAccXCalibrationAccum += nAccX;
        if (++m_nAccXCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nAccXOffset = - m_nAccXCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    int nAlignedAccX = nAccX + m_nAccXOffset;
    m_nAccXLast = FilterValue(nAlignedAccX, &m_fifoAccXFiltration);
    m_vecAccX.push_back(m_nAccXLast);
    m_nRenewStatus |= ACCX_RENEWED;
    TryFusion();
}

void DataKeeper::SetAccY(int nAccY)
{
    if (m_nAccYCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nAccYCalibrationAccum += nAccY;
        if (++m_nAccYCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nAccYOffset = - m_nAccYCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    int nAlignedAccY = nAccY + m_nAccYOffset;
    m_nAccYLast = FilterValue(nAlignedAccY, &m_fifoAccYFiltration);
    m_vecAccY.push_back(m_nAccYLast);
    m_nRenewStatus |= ACCY_RENEWED;
    TryFusion();
}

void DataKeeper::SetAccZ(int nAccZ)
{
    if (m_nAccZCalibrationCounter < SIZE_CALIBRATION_SET) {
        m_nAccZCalibrationAccum += nAccZ;
        if (++m_nAccZCalibrationCounter == SIZE_CALIBRATION_SET)
            m_nAccZOffset = UNIT_ACCELERATION - m_nAccZCalibrationAccum / SIZE_CALIBRATION_SET;
        return;
    }

    int nAlignedAccZ = nAccZ + m_nAccZOffset;
    m_nAccZLast = FilterValue(nAlignedAccZ, &m_fifoAccZFiltration);
    m_vecAccZ.push_back(m_nAccZLast);
    m_nRenewStatus |= ACCZ_RENEWED;
    TryFusion();
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
    return ((m_nReadyStatus & ACCX_READY) &&
            (m_nReadyStatus & ACCY_READY) &&
            (m_nReadyStatus & ACCZ_READY));
}

bool DataKeeper::IsGyroReady()
{
    return ((m_nRenewStatus & GYROX_RENEWED) &&
            (m_nRenewStatus & GYROY_RENEWED) &&
            (m_nRenewStatus & GYROZ_RENEWED));
}

int DataKeeper::FilterValue(int nCurrentValue, CFifo<int> *fifoFiltrationWindow)
{
    if (!DO_FILTER)
        return nCurrentValue;

    fifoFiltrationWindow->Push(nCurrentValue);
    if (fifoFiltrationWindow->IsFull()) {
        int *pWindow = fifoFiltrationWindow->GetData(NULL);
        int nLastValue = (this->*GetWindowWeightedValue)(pWindow);
        delete pWindow;
        return nLastValue;
    }

    return nCurrentValue;
}

int DataKeeper::GetWindowMedian(int *pWindow)
{
    if (pWindow == NULL)
        return 0;

    qsort((void *)pWindow, SIZE_AVERAGING_WINDOW, sizeof(*pWindow), compare);
    return pWindow[POSITION_MEDIAN];
}

int DataKeeper::GetWindowAverage(int *pWindow)
{
    if (pWindow == NULL)
        return 0;

    int nSum = 0;
    for (int i = 0; i < SIZE_AVERAGING_WINDOW; i++)
        nSum += pWindow[i];

    return nSum / SIZE_AVERAGING_WINDOW;
}

void DataKeeper::TryFusion()
{
    if ((~m_nRenewStatus & ACCX_RENEWED) ||
        (~m_nRenewStatus & ACCY_RENEWED) ||
        (~m_nRenewStatus & ACCZ_RENEWED))
        return;

    long long int nSum = m_nAccXLast * m_nAccXLast +
                         m_nAccYLast * m_nAccYLast +
                         m_nAccZLast * m_nAccZLast;
    double fLength = sqrt(nSum);
    double fProportion = (double) UNIT_ACCELERATION / fLength;

    m_nAccXLast *= fProportion;
    m_nAccYLast *= fProportion;
    m_nAccZLast *= fProportion;

    m_nReadyStatus |= ACCX_READY;
    m_nReadyStatus |= ACCY_READY;
    m_nReadyStatus |= ACCZ_READY;
}

int compare(const void * a, const void * b)
{
  if ( *(int*)a <  *(int*)b ) return -1;
  if ( *(int*)a == *(int*)b ) return 0;
  if ( *(int*)a >  *(int*)b ) return 1;
}
