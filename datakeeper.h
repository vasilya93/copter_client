#ifndef DATAKEEPER_H
#define DATAKEEPER_H

#include <vector>
#include <sys/time.h>
#include "fifo.h"
#include "vec3.h"

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

    void SetRoll(float fRoll);
    void SetPitch(float fPitch);
    void SetYaw(float fYaw);

    void GetAcc(int &nAccX, int &nAccY, int &nAccZ);
    void GetGyro(int &nGyroX, int &nGyroY, int &nGyroZ);
    void GetAccVectorUnread(std::vector<double> &vecAccX,
                            std::vector<double> &vecAccY,
                            std::vector<double> &vecAccZ);
    void GetGyroVectorUnread(std::vector<double> &vecGyroX,
                             std::vector<double> &vecGyroY,
                             std::vector<double> &vecGyroZ);

public:
    inline void ClearAccXUnread()
    {
        m_vecAccXUnread.clear();
    }
    inline void ClearAccYUnread()
    {
        m_vecAccYUnread.clear();
    }
    inline void ClearAccZUnread()
    {
        m_vecAccZUnread.clear();
    }
    inline void ClearGyroXUnread()
    {
        m_vecGyroXUnread.clear();
    }
    inline void ClearGyroYUnread()
    {
        m_vecGyroYUnread.clear();
    }
    inline void ClearGyroZUnread()
    {
        m_vecGyroZUnread.clear();
    }
public:
    inline const std::vector<double> &GetAccXUnread()
    {
        return m_vecAccXUnread;
    }
    inline const std::vector<double> &GetAccYUnread()
    {
        return m_vecAccYUnread;
    }
    inline const std::vector<double> &GetAccZUnread()
    {
        return m_vecAccZUnread;
    }
    inline const std::vector<double> &GetGyroXUnread()
    {
        return m_vecGyroXUnread;
    }
    inline const std::vector<double> &GetGyroYUnread()
    {
        return m_vecGyroYUnread;
    }
    inline const std::vector<double> &GetGyroZUnread()
    {
        return m_vecGyroZUnread;
    }
public:
    void ClearData();

    void GetDCM(vec3 &vec3ITiedCurrent, vec3 &vec3JTiedCurrent, vec3 &vec3KTiedCurrent);
    void GetAngles(float &fRoll, float &fPitch, float &fYaw);
    inline unsigned long long GetTimeGap()
    {
        return m_nTimeGap;
    }

    bool IsAccReady();
    bool IsGyroReady();
    bool AreAnglesReady();
    inline bool IsDCMReady()
    {
        return m_nRenewStatus & DCM_RENEWED;
    }

private:
    int FilterValue(int nCurrentValue, CFifo<int> *fifoFiltrationWindiow);
    int GetWindowMedian(int *pWindow);
    int GetWindowAverage(int *pWindow);
    void TryFusion();
    void TryExpose();

private:
    enum {
        ACCX_RENEWED = 1 << 0,
        ACCY_RENEWED = 1 << 1,
        ACCZ_RENEWED = 1 << 2,
        GYROX_RENEWED = 1 << 3,
        GYROY_RENEWED = 1 << 4,
        GYROZ_RENEWED = 1 << 5,
        DCM_RENEWED = 1 << 6,
        ROLL_RENEWED = 1 << 7,
        PITCH_RENEWED = 1 << 8,
        YAW_RENEWED = 1 << 9
    };
    enum {
        ACCX_NOT_EXPOSED = 1 << 0,
        ACCY_NOT_EXPOSED = 1 << 1,
        ACCZ_NOT_EXPOSED = 1 << 2,
        GYROX_NOT_EXPOSED = 1 << 3,
        GYROY_NOT_EXPOSED = 1 << 4,
        GYROZ_NOT_EXPOSED = 1 << 5
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

    std::vector<double> m_vecAccXUnread,
                        m_vecAccYUnread,
                        m_vecAccZUnread,
                        m_vecGyroXUnread,
                        m_vecGyroYUnread,
                        m_vecGyroZUnread;

    int m_nAccXLast,
        m_nAccYLast,
        m_nAccZLast,
        m_nGyroXLast,
        m_nGyroYLast,
        m_nGyroZLast;
    int m_nRenewStatus;
    int m_nExposeStatus;

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

    //----Fusion elements-----
    vec3 m_vec3ITiedCurrent;
    vec3 m_vec3ITiedPrevious;
    vec3 m_vec3JTiedCurrent;
    vec3 m_vec3JTiedPrevious;
    vec3 m_vec3KTiedCurrent;
    vec3 m_vec3KTiedPrevious;

    float m_fRollCurrent,
          m_fPitchCurrent,
          m_fYawCurrent;

    timeval m_timePrevious;
    unsigned long long m_nTimeGap;

    //----Constants----
    const int SIZE_CALIBRATION_SET = 0;
    const int SIZE_AVERAGING_WINDOW = 21;
    const unsigned int POSITION_MEDIAN = 10;
    const bool DO_FILTER = false;
    const FiltrationType FILTRATION_TYPE = FILTRATION_MEDIAN;
    const int UNIT_ACCELERATION = 8192;
};

#endif // DATAKEEPER_H
