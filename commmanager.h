#ifndef COMMMANAGER_H
#define COMMMANAGER_H

#include <QObject>
#include "SerialComm.h"
#include "packetmanager.h"
#include "datakeeper.h"

class CommManager : public QObject, protected SerialCommSubscribable
{
    Q_OBJECT
public:
    explicit CommManager(QObject *parent = 0);
    virtual ~CommManager();

    inline int Connect()
    {
        return m_pSerialComm->Connect(PORT_NAME, SerialComm::BAUDRATE_57600);
    }
    inline void SendStart()
    {
        m_pSerialComm->Write("start", 5);
    }

signals:
    void signalRenewAccel(int, int, int);
    void signalRenewGyro(int, int, int);

protected:
    virtual void SerialBytesReceivedHandler(unsigned char*, unsigned int);
    virtual void SerialNothingReceivedHandler(){}

public slots:

private:
    SerialComm *m_pSerialComm;
    PacketManager *m_pPacketManager;
    DataKeeper *m_pDataKeeper;

    const char* PORT_NAME = "/dev/ttyS0";
};

#endif // COMMMANAGER_H
