#include "ClientView.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientView w;
    w.resize(700, 300);
    w.show();

    return a.exec();
}

/* Assumptions:
 * 1. When CommManager processes data received by SerialComm it gives them to PacketManager
 * and then checks DataKeeper. It can happen that there is more than one full set of data in
 * a packet, so CommManager will read only the last, but not all data.
 */
