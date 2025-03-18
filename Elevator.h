#ifndef ELEVATOR_H
#define ELEVATOR_H

// elevator manages current floor, number of passengers, and direction
// can move up/down and check capacity

class Elevator {
public:
    Elevator(int current_floor=1, int num_passengers=0, bool going_up=true);

    int getCurrentFloor();
    int getNumPassengers();
    bool getGoingUp();

    void setCurrentFloor(int floor);
    void setNumPassengers(int num_p);
    void setGoingUp(bool val);

    void moveUp();
    void moveDown();
    void updateElevatorDisplays();
    bool checkCapactiy();

private:
    int current_floor;
    int num_passengers;
    bool going_up;

    static const int MAX_CAP = 10; // example capacity
};

#endif // ELEVATOR_H
