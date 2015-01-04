#ifndef DATAKEEPER_H
#define DATAKEEPER_H

#include <vector>

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
    enum {
        ACCX_RENEWED = 1 << 0,
        ACCY_RENEWED = 1 << 1,
        ACCZ_RENEWED = 1 << 2,
        GYROX_RENEWED = 1 << 3,
        GYROY_RENEWED = 1 << 4,
        GYROZ_RENEWED = 1 << 5
    };
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
};

#endif // DATAKEEPER_H
