#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "controller.h"

class Keyboard : public Controller {
public:
    Keyboard();
    virtual QString name() { return "Keyboard"; }
    virtual bool rusStatus() { return true; }
    virtual bool rudStatus() { return true; }
    virtual bool pedStatus() { return true; }

    virtual void fillRUSParams(TRUSParams&);
    virtual void fillRUDParams(TRUDParams&);
    virtual void fillPEDParams(TPEDParams&);
};

#endif // KEYBOARD_H
