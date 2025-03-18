#include "Passenger.h"

Passenger::Passenger(int sf, int df, int ts, int cf,
                     bool onElev, bool openBtn, bool closeBtn, bool obstruct,
                     bool gu, int en)
    : start_floor(sf)
    , dest_floor(df)
    , timestep_req(ts)
    , current_floor(cf)
    , is_on_elevator(onElev)
    , press_open_door(openBtn)
    , press_close_door(closeBtn)
    , obstruct_door(obstruct)
    , going_up(gu)
    , elev_num(en)
{
}

// getters
int  Passenger::getStartFloor()      { return start_floor; }
int  Passenger::getDestFloor()       { return dest_floor; }
int  Passenger::getTimestepRequest() { return timestep_req; }
int  Passenger::getCurrentFloor()    { return current_floor; }
bool Passenger::getIsOnElevator()    { return is_on_elevator; }
bool Passenger::getPressOpenDoor()   { return press_open_door; }
bool Passenger::getPressCloseDoor()  { return press_close_door; }
bool Passenger::getObstructDoor()    { return obstruct_door; }
bool Passenger::getGoingUp()         { return going_up; }
int  Passenger::getElevNum()         { return elev_num; }

// setters
void Passenger::setStartFloor(int sf)      { start_floor = sf; }
void Passenger::setDestFloor(int df)       { dest_floor = df; }
void Passenger::setTimestepRequest(int ts) { timestep_req = ts; }
void Passenger::setCurrentFloor(int floor) { current_floor = floor; }
void Passenger::setIsOnElevator(bool chk)  { is_on_elevator = chk; }
void Passenger::setPressOpenDoor(bool chk) { press_open_door = chk; }
void Passenger::setPressCloseDoor(bool chk){ press_close_door = chk; }
void Passenger::setObstructDoor(bool chk)  { obstruct_door = chk; }
void Passenger::setGoingUp(bool val)       { going_up = val; }
void Passenger::setElevNum(int num)        { elev_num = num; }

// updates passenger data from gui input
void Passenger::update(int sf, int df, int ts, int cf,
                       bool openDoor, bool closeDoor, bool obst, bool gu)
{
    setStartFloor(sf);
    setDestFloor(df);
    setTimestepRequest(ts);
    setCurrentFloor(cf);
    setPressOpenDoor(openDoor);
    setPressCloseDoor(closeDoor);
    setObstructDoor(obst);
    setGoingUp(gu);
}

// passenger has arrived if they're no longer on elevator
// and current_floor == dest_floor
bool Passenger::hasArrived()
{
    return (!is_on_elevator && current_floor == dest_floor);
}