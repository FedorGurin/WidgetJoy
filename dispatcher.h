#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QTimer>
#include <QUdpSocket>
#include <QHash>
#include <QPair>
#include "controllers.h"

typedef struct TIP_Node_
{
    QString ip;  //IP - адрес
    quint16 port;//порт узла
}TIP_Node;

class Dispatcher : public QObject {
    Q_OBJECT
public:
    Dispatcher();
    void setControllerType(ControllerType type);
    ~Dispatcher();
signals:
    void paramsUpdated(const TControllerParams&);
    void statusUpdated(const QHash<ControllerType, QPair<bool, bool>>);

private slots:
    void checkStatus();
    void fillParams();
    void sendParams();
    void setKeyboard(bool);
    void slotKey(int);

private:
    //! чтение IP узлов
    void readIpNodes();
    QString readParamFromXMLFile(QString fileName,QString nameProg,QString nameAttrib);
    TControllerParams _params;
    QTimer dataTimer;
    QTimer statusTimer;
    QUdpSocket udpSocket;
    Controller *rus; // current RUS controller
    Controller *rud; // current RUD controller
    QHash<ControllerType, Controller *> controllersByType;
    QVector <TIP_Node> listIp;
};

#endif // DISPATCHER_H
