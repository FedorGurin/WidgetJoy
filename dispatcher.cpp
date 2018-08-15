#include "dispatcher.h"
#include <QThread>
#include <iostream>

#define IP_UCVS "192.168.72.1"
#define PORT_UCVS 9000
Dispatcher::Dispatcher() {
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(fillParams()));
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(checkStatus()));
    dataTimer.start(10);
    statusTimer.start(100);

    controllersByType[KEYBOARD] = new Keyboard;
    controllersByType[JOYSTICK] = new Joystick;

    rus = controllersByType[JOYSTICK];
    rud = controllersByType[JOYSTICK];

    connect(this, SIGNAL(paramsUpdated(TControllerParams)), this, SLOT(sendParams()));
}
void Dispatcher::setControllerType(ControllerType type)
{
    rus = controllersByType[type];
    rud = controllersByType[type];
}
void Dispatcher::setKeyboard(bool on)
{
    if(on == true)
        setControllerType(KEYBOARD);
    else
        setControllerType(JOYSTICK);
}
void Dispatcher::slotKey(int value)
{
    _params.rusParams.x = 0.0;
    _params.rusParams.y = 0.0;
    _params.rudParams.z = 0.0;
   if(value == 2)
        _params.rusParams.x = -1.0;

   if(value == 3)
       _params.rusParams.x = 1.0;

   if(value == 0)
       _params.rusParams.y = -1.0;

   if(value == 1)
       _params.rusParams.y = 1.0;

}

Dispatcher::~Dispatcher() {
    for (auto it = controllersByType.begin(); it != controllersByType.end(); it++) {
        delete it.value();
    }
}

void Dispatcher::checkStatus() {
    QHash<ControllerType, QPair<bool, bool>> statuses;
    for (auto it = controllersByType.begin(); it != controllersByType.end(); it++) {
        bool rusS = it.value()->rusStatus();
        bool rudS = it.value()->rudStatus();
        statuses[it.key()] = QPair<bool, bool>(rusS, rudS);
    }
    emit statusUpdated(statuses);
}

void Dispatcher::fillParams() {
//    std::cout << "Dispatcher: " << QThread::currentThreadId() << std::endl;
    rus->fillRUSParams(_params.rusParams);
    rud->fillRUDParams(_params.rudParams);
    emit paramsUpdated(_params);
}

void Dispatcher::sendParams() {
    udpSocket.writeDatagram((char *)&_params, sizeof(TControllerParams), QHostAddress(IP_UCVS), PORT_UCVS);
}
