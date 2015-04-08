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
    m_nExposeStatus(0),
    m_bIsLoggingEnabled(true),
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
    m_nGyroZOffset(0),
    m_vec3ITiedPrevious(1, 0, 0),
    m_vec3JTiedPrevious(0, 1, 0),
    m_vec3KTiedPrevious(0, 0, 1),
    m_nTimeGap(0)
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
    
    gettimeofday(&m_timePrevious, NULL);
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
    if (m_bIsLoggingEnabled) {
        m_vecGyroX.push_back(m_nGyroXLast);
    }
    m_nRenewStatus |= GYROX_RENEWED;
    m_nExposeStatus |= GYROX_NOT_EXPOSED;

    TryExpose();
    //TryFusion();
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
    if (m_bIsLoggingEnabled) {
        m_vecGyroY.push_back(m_nGyroYLast);
    }
    m_nRenewStatus |= GYROY_RENEWED;
    m_nExposeStatus |= GYROY_NOT_EXPOSED;

    TryExpose();
    //TryFusion();
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
    if (m_bIsLoggingEnabled) {
        m_vecGyroZ.push_back(m_nGyroZLast);
    }
    m_nRenewStatus |= GYROZ_RENEWED;
    m_nExposeStatus |= GYROZ_NOT_EXPOSED;

    TryExpose();
    //TryFusion();
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
    if (m_bIsLoggingEnabled) {
        m_vecAccX.push_back(m_nAccXLast);
    }
    m_nRenewStatus |= ACCX_RENEWED;
    m_nExposeStatus |= ACCX_NOT_EXPOSED;

    TryExpose();
    //TryFusion();
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
    if (m_bIsLoggingEnabled) {
        m_vecAccY.push_back(m_nAccYLast);
    }
    m_nRenewStatus |= ACCY_RENEWED;
    m_nExposeStatus |= ACCY_NOT_EXPOSED;

    TryExpose();
    //TryFusion();
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
    if (m_bIsLoggingEnabled) {
        m_vecAccZ.push_back(m_nAccZLast);
    }
    m_nRenewStatus |= ACCZ_RENEWED;
    m_nExposeStatus |= ACCZ_NOT_EXPOSED;

    TryExpose();
    //TryFusion();
}

void DataKeeper::SetRoll(float fRoll)
{
    m_fRollCurrent = fRoll;
    m_nRenewStatus |= ROLL_RENEWED;
}

void DataKeeper::SetPitch(float fPitch)
{
    m_fPitchCurrent = fPitch;
    m_nRenewStatus |= PITCH_RENEWED;
}

void DataKeeper::SetYaw(float fYaw)
{
    m_fYawCurrent = fYaw;
    m_nRenewStatus |= YAW_RENEWED;
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

void DataKeeper::GetAccVectorUnread(std::vector<double> &vecAccX,
                                    std::vector<double> &vecAccY,
                                    std::vector<double> &vecAccZ)
{
    vecAccX.insert(vecAccX.end(), m_vecAccXUnread.begin(), m_vecAccXUnread.end());
    vecAccY.insert(vecAccY.end(), m_vecAccYUnread.begin(), m_vecAccYUnread.end());
    vecAccZ.insert(vecAccZ.end(), m_vecAccZUnread.begin(), m_vecAccZUnread.end());

    m_vecAccXUnread.clear();
    m_vecAccYUnread.clear();
    m_vecAccZUnread.clear();
}

void DataKeeper::GetGyroVectorUnread(std::vector<double> &vecGyroX,
                                     std::vector<double> &vecGyroY,
                                     std::vector<double> &vecGyroZ)
{
    vecGyroX.insert(vecGyroX.end(), m_vecGyroXUnread.begin(), m_vecGyroXUnread.end());
    vecGyroY.insert(vecGyroY.end(), m_vecGyroYUnread.begin(), m_vecGyroYUnread.end());
    vecGyroZ.insert(vecGyroZ.end(), m_vecGyroZUnread.begin(), m_vecGyroZUnread.end());

    m_vecGyroXUnread.clear();
    m_vecGyroYUnread.clear();
    m_vecGyroZUnread.clear();
}

void DataKeeper::ClearData()
{
    m_nRenewStatus = 0;
    m_nExposeStatus = 0;
}

void DataKeeper::GetDCM(vec3 &vec3ITiedCurrent, vec3 &vec3JTiedCurrent, vec3 &vec3KTiedCurrent)
{
    vec3ITiedCurrent = m_vec3ITiedCurrent;
    vec3JTiedCurrent = m_vec3JTiedCurrent;
    vec3KTiedCurrent = m_vec3KTiedCurrent;

    m_nRenewStatus &= ~DCM_RENEWED;
}

void DataKeeper::GetAngles(float &fRoll, float &fPitch, float &fYaw)
{
    fRoll = m_fRollCurrent;
    fPitch = m_fPitchCurrent;
    fYaw = m_fYawCurrent;
    m_nRenewStatus &= ~(ROLL_RENEWED |
                        PITCH_RENEWED |
                        YAW_RENEWED);
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

bool DataKeeper::AreAnglesReady()
{
    return ((m_nRenewStatus & ROLL_RENEWED) &&
            (m_nRenewStatus & PITCH_RENEWED) &&
            (m_nRenewStatus & YAW_RENEWED));
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
    vec3 vec3dTheta,
         vec3Aux;
    float fCorrectionLength;
    timeval timeCurrent;

    if ((~m_nRenewStatus & ACCX_RENEWED) ||
        (~m_nRenewStatus & ACCY_RENEWED) ||
        (~m_nRenewStatus & ACCZ_RENEWED) ||
        (~m_nRenewStatus & GYROX_RENEWED) ||
        (~m_nRenewStatus & GYROY_RENEWED) ||
        (~m_nRenewStatus & GYROZ_RENEWED))
        return;
    
    gettimeofday(&timeCurrent, NULL);
    m_nTimeGap = (timeCurrent.tv_sec - m_timePrevious.tv_sec) * 1000000 +
              timeCurrent.tv_usec - m_timePrevious.tv_usec;
    m_timePrevious = timeCurrent;

    m_vec3KTiedCurrent.x = m_nAccXLast;
    m_vec3KTiedCurrent.y = m_nAccYLast;
    m_vec3KTiedCurrent.z = m_nAccZLast;
    m_vec3KTiedCurrent.normalize();

    vec3dTheta = m_vec3KTiedPrevious.cross(m_vec3KTiedCurrent - m_vec3KTiedPrevious);

    //find new vectors from angle adn normalize them
    m_vec3ITiedCurrent = m_vec3ITiedPrevious + (vec3dTheta.cross(m_vec3ITiedPrevious));
    m_vec3ITiedCurrent.normalize();
    m_vec3JTiedCurrent = m_vec3JTiedPrevious + (vec3dTheta.cross(m_vec3JTiedPrevious));
    m_vec3JTiedCurrent.normalize();

    //correct found vectors so that they are perpendicular to K vector
    vec3Aux = m_vec3KTiedCurrent.cross(m_vec3ITiedCurrent);
    m_vec3ITiedCurrent = vec3Aux.cross(m_vec3KTiedCurrent);
    vec3Aux = m_vec3KTiedCurrent.cross(m_vec3JTiedCurrent);
    m_vec3JTiedCurrent = vec3Aux.cross(m_vec3KTiedCurrent);

    //correct vectors so that they are perpendicular to each other
    vec3Aux = m_vec3ITiedCurrent;
    fCorrectionLength = m_vec3JTiedCurrent.dot(m_vec3ITiedCurrent) / 2;
    m_vec3ITiedCurrent = m_vec3ITiedCurrent - (m_vec3JTiedCurrent * fCorrectionLength);
    m_vec3JTiedCurrent = m_vec3JTiedCurrent - (vec3Aux * fCorrectionLength);

    m_fRollCurrent = asin(m_vec3KTiedCurrent.y);
    m_fPitchCurrent = atan2(-m_vec3KTiedCurrent.x, m_vec3KTiedCurrent.z);
    m_fYawCurrent = atan2(-m_vec3ITiedCurrent.y, m_vec3JTiedCurrent.y);

    m_vec3ITiedPrevious = m_vec3ITiedCurrent;
    m_vec3JTiedPrevious = m_vec3JTiedCurrent;
    m_vec3KTiedPrevious = m_vec3KTiedCurrent;

    m_nRenewStatus |= DCM_RENEWED;
    m_nRenewStatus |= ROLL_RENEWED | PITCH_RENEWED | YAW_RENEWED;
}

void DataKeeper::TryExpose()
{
    if ((~m_nExposeStatus & ACCX_NOT_EXPOSED) ||
        (~m_nExposeStatus & ACCY_NOT_EXPOSED) ||
        (~m_nExposeStatus & ACCZ_NOT_EXPOSED) ||
        (~m_nExposeStatus & GYROX_NOT_EXPOSED) ||
        (~m_nExposeStatus & GYROY_NOT_EXPOSED) ||
        (~m_nExposeStatus & GYROZ_NOT_EXPOSED))
        return;

    m_vecAccXUnread.push_back((double)m_nAccXLast);
    m_vecAccYUnread.push_back((double)m_nAccYLast);
    m_vecAccZUnread.push_back((double)m_nAccZLast);
    m_vecGyroXUnread.push_back((double)m_nGyroXLast);
    m_vecGyroYUnread.push_back((double)m_nGyroYLast);
    m_vecGyroZUnread.push_back((double)m_nGyroZLast);

    m_nExposeStatus = 0;
}

int compare(const void * a, const void * b)
{
  if ( *(int*)a <  *(int*)b ) return -1;
  else if ( *(int*)a == *(int*)b ) return 0;
  else return 1;
}
