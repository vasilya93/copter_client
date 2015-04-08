#ifndef COMMMANAGER_H
#define COMMMANAGER_H

#include <QObject>
#include <QVector>
#include "SerialComm.h"
#include "packetmanager.h"
#include "datakeeper.h"
#include "logwriter.h"
#include "vec3.h"

class CommManager : public QObject, protected SerialCommSubscribable
{
    Q_OBJECT
public:
    explicit CommManager(QObject *parent = 0);
    virtual ~CommManager();

    inline int Connect()
    {
        return m_pSerialComm->Connect(PORT_NAME, SerialComm::BAUDRATE_115200);
    }
    inline void SendStart()
    {
        m_pSerialComm->Write("start", 5);
    }
    inline DataKeeper &GetDataKeeper()
    {
        return *m_pDataKeeper;
    }

public:
    void GetAccUnread(QVector<double> &vecAccX, QVector<double> &vecAccY, QVector<double> &vecAccZ);
    void GetGyroUnread(QVector<double> &vecGyroX, QVector<double> &vecGyroY, QVector<double> &vecGyroZ);

    void StartConnection();
    void WriteLog();

protected:
    virtual void SerialBytesReceivedHandler(unsigned char*, unsigned int);
    virtual void SerialNothingReceivedHandler(){}

private:
    SerialComm *m_pSerialComm;
    PacketManager *m_pPacketManager;
    DataKeeper *m_pDataKeeper;
    LogWriter m_logWriter;

    const char* PORT_NAME = "/dev/ttyS0";
};

#endif // COMMMANAGER_H
