#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "controller.h"

#define NBTNS 28 // Максимальное число кнопок
#define NAXES 7 // Максимальное число осей
//#define JOYNAME "Thrustmaster" // Имя джойстика
#define JOYNAMELEN 32 // Длина ID-строки (имени) джойстика

/*
 * Джойстик Thrustmaster Hotas Cougar (РУС + РУД)
 */
class Joystick: public Controller {
private:
    // Статус РУС
    bool rusWork;
    // Статус РУД
    bool rudWork;
    //! статус педалей
    bool pedWork;

    void checkStatus(std::string dev,std::string name,bool &oou, int& fd);
    TControllerParams _params;
    virtual void fillParams();
#ifdef __linux__
    void l_fillParamsRus(int fd, bool& work);
    void l_fillParamsRud(int fd, bool& work);
    void l_fillParamsPed(int fd, bool& work);
#endif
#ifdef WIN32
    void w_fillParams();
    void k_fillParams();
#endif
public:
    Joystick();
    ~Joystick();
    int fdRus = 0; // joystick device file descriptor
    int fdRud = 0 ;
    int fdPed = 0;

    virtual QString name() { return "Thrustmaster"; }
    virtual bool rusStatus();
    virtual bool rudStatus();
    virtual bool pedStatus();

    virtual void fillRUSParams(TRUSParams&);
    virtual void fillRUDParams(TRUDParams&);
    virtual void fillPEDParams(TPEDParams&);
};

#endif // JOYSTICK_H
