Jasmine Jamali
101156769
COMP3004
Assignment 3

                       	 	ELEVATOR SYSTEM SIMULATOR

Youtube Link: https://www.youtube.com/watch?v=kvMUCOnvZw0



How to Run the Program:

1. Open the project in Qt Creator

2. Build the project

3. Run the executable from Qt Creator or CL


What it does:

- Simulates elevator system w/ multiple elevators and passengers

- Passengers request elevators at certain timesteps

- Supports emergency scenarios

- Robust-ish UI


UML Sequence Diagrams

1. Success Scenario 1:

- Three passengers, five floors, and two elevators

- Passengers 1 and 2 start on floor 1 and go to floor 2

- Passenger 3 stays on floor 1

- The simulation is set up, requests are processed, and then stopped

2. Success Scenario 2:

- Three passengers, five floors, and two elevators

- Passenger 1 goes from floor 1 to 2 and interacts with the elevator (door obstructions, open/close buttons)

- Passengers 2 and 3 are inactive

- The simulation runs until idle

3. Safety Scenario 1:

- Three passengers, five floors, and two elevators

- Passenger 1 moves from floor 5 to floor 2

- The simulation is interrupted by a fire event

4. Safety Scenario 2:

- Three passengers, five floors, and two elevators

- Passenger 1 moves from floor 5 to floor 2

- Passenger 3 moves from floor 2 to 4 at timestep 4

- A power outage interrupts the system