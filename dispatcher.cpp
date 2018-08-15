#include "dispatcher.h"
#include <QThread>
#include <iostream>
#include <QFile>
#include <QApplication>
#include <QDomElement>
#define IP_UCVS "192.168.72.1"
#define PORT_UCVS 9000

QString Dispatcher::readParamFromXMLFile(QString fileName,QString nameProg,QString nameAttrib)
{
    bool openFile=false;
    QString tempName("");
    QFile file(qApp->applicationDirPath()+"/"+fileName);
    QDomDocument domDoc;
    openFile = file.open(QIODevice::ReadOnly);
    if(openFile==true)
    {
        bool readXML=false;
        readXML=domDoc.setContent(&file,true);
        if(readXML==true)
        {
            QDomElement ele1    = domDoc.firstChildElement("data");
            QDomElement ele     = ele1.firstChildElement(nameProg);
            QString tempName    = ele.attribute(nameAttrib,"");
            return tempName;
        }
    }else
        qDebug("ReadParamFromXMLFile: File not find=%s\n nameProg=%s\n nameAttrib=%s\n",qPrintable(fileName),qPrintable(nameProg),qPrintable(nameAttrib));

    return tempName;
}
Dispatcher::Dispatcher()
{

    listIp.clear();
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(fillParams()));
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(checkStatus()));
    dataTimer.start(10);
    statusTimer.start(100);

    controllersByType[KEYBOARD] = new Keyboard;
    controllersByType[JOYSTICK] = new Joystick;

    rus = controllersByType[JOYSTICK];
    rud = controllersByType[JOYSTICK];

    connect(this, SIGNAL(paramsUpdated(TControllerParams)), this, SLOT(sendParams()));

    readIpNodes();
}
void Dispatcher::readIpNodes()
{
    TIP_Node node;
    QString valueIp="",valuePort="", numNode="NODE_";
    int i = 0;
    do
    {
        numNode = "NODE_" + QString::number(i);

        valueIp     = readParamFromXMLFile("setting.xml",numNode,"IP");
        valuePort   = readParamFromXMLFile("setting.xml",numNode,"PORT");
        if(valueIp!="" && valuePort!="")
        {
            node.ip = valueIp;
            node.port = valuePort.toInt();
            listIp.push_back(node);
        }

        i++;
    }while( valueIp!="" && valuePort!="");
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

void Dispatcher::sendParams()
{
    if(listIp.isEmpty())
        udpSocket.writeDatagram((char *)&_params, sizeof(TControllerParams), QHostAddress(IP_UCVS), PORT_UCVS);
    else
    {
        for(int i = 0; i<listIp.size();i++)
        {
            udpSocket.writeDatagram((char *)&_params, sizeof(TControllerParams), QHostAddress(listIp[i].ip), listIp[i].port);
        }
    }
}
