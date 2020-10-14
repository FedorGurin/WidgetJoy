#include "controller.h"

TControllerParamsB paramsToBitwise(TControllerParams &p) {
    TRUSParamsB rusb;
    rusb.x = p.rusParams.x;
    rusb.y = p.rusParams.y;
    // TODO: Buttons
    TRUDParamsB rudb;
    rudb.z = p.rudParams.z;
    rudb.man_rng = p.rudParams.man_rng;
    rudb.ant_elev = p.rudParams.ant_elev;
    // TODO: Buttons
    //return TControllerParamsB{rusb, rudb};
    TControllerParamsB rus;
    return rus;
}

Controller::Controller() {

}

Controller::~Controller() {

}
