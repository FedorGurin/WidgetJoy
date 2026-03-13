#include "joystick.h"
#include <QGamepad>
#include <QtGlobal>


#ifdef WIN32
#include <Windows.h>
#endif

Joystick::Joystick() {
    gamepad.setDeviceId(1);
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
void Joystick::setRUSMinMax(TMinMaxAxis &p)
{
    rusMinMax = p;
}
void Joystick::setRUDMinMax(TMinMaxAxis &p)
{
    rudMinMax = p;
}
void Joystick::setPEDMinMax(TMinMaxAxis &p)
{
    pedMinMax = p;
}
void Joystick::fillParams() {
    w_fillParams();
    k_fillParams();
}
void Joystick::w_fillParams()
{
    int v = gamepad.deviceId();
    QList<int> list = QGamepadManager::instance()->connectedGamepads();
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
        _params.rudParams.enable = gamepad.buttonX();
        _params.rudParams.uncage = gamepad.buttonB();

        return;
    }else
    {

        // JOYINFO joyinfo;
        // if(joyGetPos(JOYSTICKID1, &joyinfo) == JOYERR_NOERROR) {
        //     _params.rusParams.x = -(2 * joyinfo.wXpos / 65535.0 - 1.0);
        //     _params.rusParams.y = (2 * joyinfo.wYpos / 65535.0 - 1.0);
        //     _params.rudParams.z = (1.0 - 1.0 * joyinfo.wZpos / 65535.0);
        //     _params.rusParams.wpn_rel = (joyinfo.wButtons&0x1)?1:0;
        //     _params.rusParams.nws= (joyinfo.wButtons&0x2)?1:0;
        //     _params.rusParams.thumb.hpos = (joyinfo.wButtons&0x4)?UP:CENTER;
        //     if(_params.rusParams.thumb.hpos == CENTER)
        //         _params.rusParams.thumb.hpos = (joyinfo.wButtons&0x8)?DOWN:CENTER;
        //     rusWork = true;
        // } else {
        //     rusWork = false;
        // }
         JOYINFOEX joyinfoex;
         if(joyGetPosEx(JOYSTICKID1, &joyinfoex) == JOYERR_NOERROR) {
             _params.rusParams.x = -(2 * joyinfoex.dwXpos / rusMinMax.axi[0].center - 1.0);
             _params.rusParams.y = (2 * joyinfoex.dwYpos / rusMinMax.axi[1].center - 1.0);
             _params.rudParams.z = (1.0 - 1.0 * joyinfoex.dwZpos/rudMinMax.axi[0].center);
             _params.pedParams.x =  -(2 * joyinfoex.dwRpos / pedMinMax.axi[0].center - 1.0);//(1.0 - 1.0 * joyinfoex.dwRpos/pedMinMax.axi[0].center);
             _params.pedParams.y =  (1.0 - 1.0 * joyinfoex.dwUpos/pedMinMax.axi[1].center);

             //_params.pedParams.z = joyinfoex.dwUpos;
             //S_params.pedParams.y = joyinfoex.dwVpos ;

             _params.rusParams.wpn_rel = (joyinfoex.dwButtons&0x1)?1:0;
             _params.rusParams.nws= (joyinfoex.dwButtons&0x2)?1:0;
             _params.rusParams.thumb.hpos = (joyinfoex.dwButtons&0x4)?UP:CENTER;
             if(_params.rusParams.thumb.hpos == CENTER)
                     _params.rusParams.thumb.hpos = (joyinfoex.dwButtons&0x8)?DOWN:CENTER;
                rusWork = true;
                pedWork = true;
         }
         else{
                 //rusWork = false;
         }

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

