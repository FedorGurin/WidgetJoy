#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "controller.h"
#include <QGamepad>
#define NBTNS 28 // Максимальное число кнопок
#define NAXES 7 // Максимальное число осей
//#define JOYNAME "Thrustmaster" // Имя джойстика
#define JOYNAMELEN 32 // Длина ID-строки (имени) джойстика

/*
 * Джойстик Thrustmaster Hotas Cougar (РУС + РУД)
 */
class Joystick: public Controller {
private:
    QGamepad gamepad;
    // Статус РУС
    bool rusWork;
    // Статус РУД
    bool rudWork;
    //! статус педалей
    bool pedWork;

    void checkStatus(std::string dev,std::string name,bool &oou, int& fd);
    //! параметры
    TControllerParams _params;
    // заполнение параметров
    virtual void fillParams();

    void w_fillParams();
    void k_fillParams();

public:
    Joystick();
    ~Joystick();
    int fdRus = 0;
    int fdRud = 0 ;
    int fdPed = 0;

    virtual QString name() { return "Thrustmaster"; }
    virtual bool rusStatus(){return rusWork;}
    virtual bool rudStatus(){return rudWork;}
    virtual bool pedStatus(){return pedWork;}

    virtual void fillRUSParams(TRUSParams&);
    virtual void fillRUDParams(TRUDParams&);
    virtual void fillPEDParams(TPEDParams&);
};

#endif // JOYSTICK_H
