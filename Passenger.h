#ifndef PASSENGER_H
#define PASSENGER_H

// passenger represents one person, can store start/dest floors,
// requested time step, door actions, etc.

class Passenger {
public:
    Passenger(int start_floor=1, int dest_floor=1, int timestep_req=1,
              int current_floor=1, bool is_on_elev=false, bool press_open=false,
              bool press_close=false, bool obstruct=false, bool going_up=true,
              int elev_num=-1);

    // getters
    int getStartFloor();
    int getDestFloor();
    int getTimestepRequest();
    int getCurrentFloor();
    bool getIsOnElevator();
    bool getPressOpenDoor();
    bool getPressCloseDoor();
    bool getObstructDoor();
    bool getGoingUp();
    int getElevNum();

    // setters
    void setStartFloor(int sf);
    void setDestFloor(int df);
    void setTimestepRequest(int ts);
    void setCurrentFloor(int floor);
    void setIsOnElevator(bool check);
    void setPressOpenDoor(bool check);
    void setPressCloseDoor(bool check);
    void setObstructDoor(bool check);
    void setGoingUp(bool val);
    void setElevNum(int num);

    // updates passenger data from gui input
    void update(int sf, int df, int ts, int cf,
                bool openDoor, bool closeDoor, bool obstruct, bool gu);

    // returns true if passenger is off elevator at their dest
    bool hasArrived();

private:
    int start_floor;
    int dest_floor;
    int timestep_req;
    int current_floor;
    bool is_on_elevator;
    bool press_open_door;
    bool press_close_door;
    bool obstruct_door;
    bool going_up;
    int elev_num;
};

#endif // PASSENGER_H