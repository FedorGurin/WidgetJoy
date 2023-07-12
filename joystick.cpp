#include "joystick.h"
#include <QGamepad>
#include <QtGlobal>


Joystick::Joystick() {
    rusWork = rudWork = pedWork = false;
    fdRus   = fdRud   = fdPed = 0;
    memset((void*)&_params,0,sizeof(_params));
}

Joystick::~Joystick() {
}
void Joystick::fillRUSParams(TRUSParams &p) {
    fillParams();
    p = _params.rusParams;
}
void Joystick::fillPEDParams(TPEDParams &p) {
    fillParams();
    p = _params.pedParams;
}
void Joystick::fillRUDParams(TRUDParams &p) {
    fillParams();
    p = _params.rudParams;
}

void Joystick::fillParams() {
    w_fillParams();
    k_fillParams();
}
void Joystick::w_fillParams()
{
    if(gamepad.isConnected() == true)
    {
        pedWork = true;
        rusWork = true;

        _params.rusParams.x = gamepad.axisLeftX();
        _params.rusParams.y = gamepad.axisLeftY();

        _params.pedParams.x = gamepad.axisRightX();
        _params.pedParams.y = gamepad.axisRightY();
        _params.rudParams.z = gamepad.buttonL2() - gamepad.buttonR2();
        _params.rudParams.z = qBound(-1.0f,_params.rudParams.z,1.0f);

        return;
    }

}
void Joystick::k_fillParams()
{
    if(rusWork == false)
    {
        _params.rusParams.x = 0.0;
        _params.rusParams.y = 0.0;
        _params.rudParams.z = 0.0;
    }


}

