#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "controller.h"

#define NBTNS 28 // Максимальное число кнопок
#define NAXES 7 // Максимальное число осей
#define JOYNAME "Thrustmaster" // Имя джойстика
#define JOYNAMELEN 12 // Длина ID-строки (имени) джойстика

/*
 * Джойстик Thrustmaster Hotas Cougar (РУС + РУД)
 */
class Joystick: public Controller {
private:
    // Статус РУС
    bool rusWork;
    // Статус РУД
    bool rudWork;
    void checkStatus();
    TControllerParams _params;
    virtual void fillParams();
#ifdef __linux__
    void l_fillParams();
#endif
#ifdef WIN32
    void w_fillParams();
    void k_fillParams();
#endif
public:
    Joystick();
    ~Joystick();
    virtual QString name() { return "Thrustmaster"; }
    virtual bool rusStatus();
    virtual bool rudStatus();
    virtual void fillRUSParams(TRUSParams&);
    virtual void fillRUDParams(TRUDParams&);
};

#endif // JOYSTICK_H
