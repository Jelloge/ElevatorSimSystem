#include "Elevator.h"

Elevator::Elevator(int floor, int num_p, bool gu)
    : current_floor(floor), num_passengers(num_p), going_up(gu)
{
}

// getters
int Elevator::getCurrentFloor() { return current_floor; }
int Elevator::getNumPassengers(){ return num_passengers; }
bool Elevator::getGoingUp()     { return going_up; }

// setters
void Elevator::setCurrentFloor(int floor) { current_floor = floor; }
void Elevator::setNumPassengers(int num_p){ num_passengers = num_p; }
void Elevator::setGoingUp(bool val)       { going_up = val; }

void Elevator::moveUp()
{
    setCurrentFloor(current_floor + 1);
}

void Elevator::moveDown()
{
    setCurrentFloor(current_floor - 1);
}

bool Elevator::checkCapactiy()
{
    // returns true if adding a passenger would exceed max
    return (num_passengers + 1 >= MAX_CAP);
}