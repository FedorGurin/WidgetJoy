#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller_types.h"
#include <QString>


TControllerParamsB paramsToBitwise(TControllerParams&);

class Controller
{
public:
    Controller();
    virtual ~Controller();
    virtual void fillRUSParams(TRUSParams&) = 0;
    virtual void fillRUDParams(TRUDParams&) = 0;
    virtual bool rusStatus() = 0;
    virtual bool rudStatus() = 0;
    virtual QString name() { return "Generic Controller";}
protected:
    unsigned cntr; // Счетчик вызовов fillParams
};

#endif // CONTROLLER_H
