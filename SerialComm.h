#ifndef SERIALCOMM_H
#define SERIALCOMM_H

#define COMM_EM_OK 0
#define COMM_EM_NOSERSEL 1
#define COMM_EM_CNTOPNSER 2
#define COMM_EM_GTSTFL 3
#define COMM_EM_STSTFL 4
#define COMM_EM_STTMFL 5
#define COMM_EM_THRCRFL 6
#define COMM_EM_TTHRCRFL 7

#define READ_BUF_SIZE 1024
#define MAX_HANDLERS_NUMBER 5

#include "thread"
#include "mutex"
//#include "Log.h"

class SerialCommSubscribable
{
public:
    SerialCommSubscribable(){}
    ~SerialCommSubscribable(){}

    virtual void SerialBytesReceivedHandler(unsigned char*, unsigned int) = 0;
    virtual void SerialNothingReceivedHandler() = 0;
};

class SerialComm
{
public:
    enum {
        BAUDRATE_9600,
        BAUDRATE_57600,
        BAUDRATE_115200,
    };

private:
    //-----------------------------Can delete after -----------------------------

    unsigned int _packetCounter;
    unsigned long _internalHigh;


    //-----------------------------Read/write common -----------------------------

    int nPortHandle;

    //-------------------------------Read elements -------------------------------
    time_t _firstTick;  //data for actual baudrate calculation, single transaction
    time_t _lastTick;

    bool _isReadingContinued; //thread data
    std::mutex _readMutex;

    unsigned short ReadBufSize; //buffer data
    unsigned char* ReadBuf;

    //-------------------------------Write elements -------------------------------

    //OVERLAPPED _writeSync;
    std::mutex _writeMutex; //thread data
    bool *_isLineUsed; //means line transferred to be transmitted
    char *m_sWriteBuf;
    unsigned int nWriteBufSize;

    void _writeThread();
    void _readThread();

public:
//-----------------------------Linked objects -----------------------------

    //Log* CommLog;

    SerialCommSubscribable* HandlersHosts[MAX_HANDLERS_NUMBER];
    unsigned short HandlersNum;

    SerialComm();
    ~SerialComm();

    unsigned char* GetReadBuf(){return ReadBuf;}

    bool AttachHandlerHost(SerialCommSubscribable* newHost);
    int Connect(const char *port, int baudrate);
    void Disconnect();

    int Write(char* line, unsigned long lineSize);
};

#endif
