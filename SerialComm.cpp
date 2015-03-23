#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "assert.h"
#include "thread"
#include "SerialComm.h"


using namespace std;

//---------------------------------------Constructors --------------------------------------

SerialComm::SerialComm() :
    _packetCounter(0),
    nPortHandle(-1),
    _firstTick(0),
    _lastTick(0),
    _isReadingContinued(false),
    ReadBufSize(READ_BUF_SIZE + 1),
    HandlersNum(0)
{
    ReadBuf = new unsigned char[ReadBufSize];
}

//---------------------------------------Destructors --------------------------------------

SerialComm::~SerialComm()
{
    Disconnect();
    delete ReadBuf;
}

//---------------------------------------Access funcs --------------------------------------


/*bool SerialComm::ResetTick(unsigned long* firstTick, unsigned long* lastTick)
{
    if (firstTick==NULL || lastTick==NULL)
    {
        return false;
    }
    *firstTick = _firstTick;
    *lastTick = _lastTick;
    _firstTick = 0;
    _lastTick = 0;
    return true;
}*/

//-----------------------------------------Functions ----------------------------------------

bool SerialComm::AttachHandlerHost(SerialCommSubscribable* newHost)
{
    if(HandlersNum > MAX_HANDLERS_NUMBER) {
        //CommLog->AddMessage("SerialComm.AttachHandlerHost: handlers' pool is full.\r\n");
        return false;
    }
    HandlersHosts[HandlersNum++] = newHost;
    //CommLog->AddMessage("SerialComm.AttachHandlerHost: handler is attached.\r\n");
    return true;
}

int SerialComm::Connect(const char* port, int baudrate)
{
    if (!(strcmp("",port))) {
        //CommLog->AddMessage("SerialComm.Connect: port name is empty.\r\n");
        return COMM_EM_NOSERSEL;
    }
    nPortHandle = open(port, O_RDWR| O_NOCTTY);
    if(nPortHandle == -1) { //does file belongs to the user?
        //CommLog->AddMessage("SerialComm.Connect: failed to create serial port file.\r\n");
        return COMM_EM_CNTOPNSER;
    }

    struct termios tty;
    struct termios tty_old;
    memset (&tty, 0, sizeof tty);

    /* Error Handling */
    if (tcgetattr(nPortHandle, &tty)) {
        printf("Error %u from tcgetattr %s\n", errno, strerror(errno));
        return errno;
    }

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    switch (baudrate) {
    case BAUDRATE_9600:
        cfsetospeed (&tty, (speed_t)B9600);
        cfsetispeed (&tty, (speed_t)B9600);
        break;
    case BAUDRATE_57600:
        cfsetospeed (&tty, (speed_t)B57600);
        cfsetispeed (&tty, (speed_t)B57600);
        break;
    case BAUDRATE_115200:
        cfsetospeed (&tty, (speed_t)B115200);
        cfsetispeed (&tty, (speed_t)B115200);
        break;
    }

    /* Setting other Port Stuff */
    tty.c_cflag     &=  ~PARENB;        // Make 8n1
    tty.c_cflag     &=  ~CSTOPB;
    tty.c_cflag     &=  ~CSIZE;
    tty.c_cflag     |=  CS8;

    tty.c_cflag     &=  ~CRTSCTS;       // no flow control
    tty.c_cc[VMIN]      =   0;                  // read doesn't block
    tty.c_cc[VTIME]     =   0;                  // 0.1 seconds read timeout
    tty.c_cflag     |=  CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush( nPortHandle, TCIFLUSH );
    if (tcsetattr(nPortHandle, TCSANOW, &tty)) {
        //TODO: check close result
        close(nPortHandle);
        nPortHandle = -1;
        printf("Error %u from tcsetattr %s\n", errno, strerror(errno));
        return -1;
    }

    _isReadingContinued = true;

    std::thread readThread(&SerialComm::_readThread, std::ref(*this));
    readThread.detach();

    //CommLog->AddMessage("SerialComm.Connect: connection is completed.\r\n");
    return true;
}

void SerialComm::Disconnect()
{
    if(nPortHandle != -1) {
        _isReadingContinued = false;
        _readMutex.lock();
        _readMutex.unlock();
        _writeMutex.lock(); //to wait for the mutex to be released
        _writeMutex.unlock();
        //TODO: check close return value
        close(nPortHandle);
        nPortHandle = -1;
        //CommLog->AddMessage("SerialComm.Disconnect: port is disconnected.\r\n");
        return;
    }

    //CommLog->AddMessage("SerialComm.Disconnect: invalid port handler.\r\n");
    return;
}

int SerialComm::Write(char* line, unsigned long lineSize)
{
    if (nPortHandle == -1)
        return -1;

    write(nPortHandle, line, lineSize);

    //sCommLog->AddMessage("SerialComm.Write: successfully completed.\r\n");
    return 0;
}

//---------------------------------------Threads --------------------------------------

void SerialComm::_readThread()
{
    int nBytesRead;
    _readMutex.lock();
    while(_isReadingContinued)
    {
        nBytesRead = read(nPortHandle, ReadBuf, ReadBufSize);
        if (nBytesRead < 0) {
           //CommLog->AddMessage("SerialComm._readThread: GetOverlappedResult returned zero.\r\n");
        }

        if(nBytesRead > 0)
        {
            time(&_lastTick);
            //CommLog->AddMessage("SerialComm._readThread: bytes received.\r\n");
            if (_firstTick == 0) {
                _firstTick = _lastTick;
            }
            ReadBuf[nBytesRead] = '\0';
            for (unsigned int i = 0; i < HandlersNum; i++) {
                HandlersHosts[i]->SerialBytesReceivedHandler(ReadBuf, nBytesRead);
            }
        } else {
            for (unsigned int i = 0; i < HandlersNum; i++) {
                HandlersHosts[i]->SerialNothingReceivedHandler();
            }
        }
    }
    _readMutex.unlock();
    return;
}

