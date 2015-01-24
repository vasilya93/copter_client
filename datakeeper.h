#ifndef DATAKEEPER_H
#define DATAKEEPER_H

#include <vector>
#include "fifo.h"

class DataKeeper
{
public:
    DataKeeper();
    ~DataKeeper();

    void SetGyroX(int nGyroX);
    void SetGyroY(int nGyroY);
    void SetGyroZ(int nGyroZ);

    void SetAccX(int nAccX);
    void SetAccY(int nAccY);
    void SetAccZ(int nAccZ);

    void GetAcc(int &nAccX, int &nAccY, int &nAccZ);
    void GetGyro(int &nGyroX, int &nGyroY, int &nGyroZ);

    bool IsAccReady();
    bool IsGyroReady();

private:
    int FilterValue(int nCurrentValue, CFifo<int> *fifoFiltrationWindiow);
    int GetWindowMedian(int *pWindow);
    int GetWindowAverage(int *pWindow);
    void TryFusion();

private:
    enum {
        ACCX_RENEWED = 1 << 0,
        ACCY_RENEWED = 1 << 1,
        ACCZ_RENEWED = 1 << 2,
        GYROX_RENEWED = 1 << 3,
        GYROY_RENEWED = 1 << 4,
        GYROZ_RENEWED = 1 << 5
    };
    enum {
        ACCX_READY = 1 << 0,
        ACCY_READY = 1 << 1,
        ACCZ_READY = 1 << 2
    };
    enum FiltrationType {
        FILTRATION_MEDIAN,
        FILTRATION_AVERAGE
    };

    //----Essential elements----
    std::vector<int> m_vecAccX,
                     m_vecAccY,
                     m_vecAccZ,
                     m_vecGyroX,
                     m_vecGyroY,
                     m_vecGyroZ;
    int m_nAccXLast,
        m_nAccYLast,
        m_nAccZLast,
        m_nGyroXLast,
        m_nGyroYLast,
        m_nGyroZLast;
    int m_nRenewStatus;
    int m_nReadyStatus;

    //----Calibration elements----
    int m_nAccXCalibrationAccum,
        m_nAccYCalibrationAccum,
        m_nAccZCalibrationAccum,
        m_nGyroXCalibrationAccum,
        m_nGyroYCalibrationAccum,
        m_nGyroZCalibrationAccum;

    int m_nAccXCalibrationCounter,
        m_nAccYCalibrationCounter,
        m_nAccZCalibrationCounter,
        m_nGyroXCalibrationCounter,
        m_nGyroYCalibrationCounter,
        m_nGyroZCalibrationCounter;

    int m_nAccXOffset,
        m_nAccYOffset,
        m_nAccZOffset,
        m_nGyroXOffset,
        m_nGyroYOffset,
        m_nGyroZOffset;

    //----Filtration elements----
    CFifo<int> m_fifoAccXFiltration,
               m_fifoAccYFiltration,
               m_fifoAccZFiltration,
               m_fifoGyroXFiltration,
               m_fifoGyroYFiltration,
               m_fifoGyroZFiltration;

    int (DataKeeper::*GetWindowWeightedValue)(int *);

    //----Constants----
    const int SIZE_CALIBRATION_SET = 200;
    const int SIZE_AVERAGING_WINDOW = 21;
    const unsigned int POSITION_MEDIAN = 10;
    const bool DO_FILTER = false;
    const FiltrationType FILTRATION_TYPE = FILTRATION_AVERAGE;
    const int UNIT_ACCELERATION = 16384;
};

#endif // DATAKEEPER_H
