#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPushButton>
#include <QString>

// constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , num_floors(1)
    , num_elev(1)
    , num_pass(1)
    , curr_ts(0)
{
    ui->setupUi(this);

    // i used to set up QSoundEffect here, but now it's removed
    // i'll just do "mock audio" in the fire(), powerOutage(), and help() methods for now

    setUpSim();
}

// destructor
MainWindow::~MainWindow()
{
    delete ui;
}

// connects signals from ui to appropriate slots
void MainWindow::connectAllSlots()
{
    connect(ui->sim_submit,       &QPushButton::released, this, &MainWindow::submitSim);
    connect(ui->pass_submit,      &QPushButton::released, this, &MainWindow::submitPass);
    connect(ui->start_sim_button, &QPushButton::released, this, &MainWindow::startSim);
    connect(ui->stop_sim_button,  &QPushButton::released, this, &MainWindow::stopSim);
    connect(ui->pause_sim_button, &QPushButton::released, this, &MainWindow::pauseSim);
    connect(ui->cont_sim_button,  &QPushButton::released, this, &MainWindow::contSim);
    connect(ui->reset_sim_button, &QPushButton::released, this, &MainWindow::resetSimulationSetup);
    connect(ui->reset_passenger_button, &QPushButton::released, this, &MainWindow::resetPassengerSetup);

    connect(&timer, &QTimer::timeout, this, &MainWindow::advanceTimestep);

    connect(ui->fire_button,       &QPushButton::released, this, &MainWindow::fire);
    connect(ui->power_out_button,  &QPushButton::released, this, &MainWindow::powerOutage);
    connect(ui->help_button,       &QPushButton::released, this, &MainWindow::help);
}

// disconnects signals so i can reset the sim safely
void MainWindow::disconnectAllSlots()
{
    disconnect(ui->sim_submit,       &QPushButton::released, this, &MainWindow::submitSim);
    disconnect(ui->pass_submit,      &QPushButton::released, this, &MainWindow::submitPass);
    disconnect(ui->start_sim_button, &QPushButton::released, this, &MainWindow::startSim);
    disconnect(ui->stop_sim_button,  &QPushButton::released, this, &MainWindow::stopSim);
    disconnect(ui->pause_sim_button, &QPushButton::released, this, &MainWindow::pauseSim);
    disconnect(ui->cont_sim_button,  &QPushButton::released, this, &MainWindow::contSim);

    disconnect(&timer, &QTimer::timeout, this, &MainWindow::advanceTimestep);

    disconnect(ui->fire_button,      &QPushButton::released, this, &MainWindow::fire);
    disconnect(ui->power_out_button, &QPushButton::released, this, &MainWindow::powerOutage);
    disconnect(ui->help_button,      &QPushButton::released, this, &MainWindow::help);
}

// disable all relevant buttons to prevent conflicting commands
void MainWindow::disableAllButtons()
{
    ui->sim_submit->setEnabled(false);
    ui->pass_submit->setEnabled(false);
    ui->start_sim_button->setEnabled(false);
    ui->stop_sim_button->setEnabled(false);
    ui->pause_sim_button->setEnabled(false);
    ui->cont_sim_button->setEnabled(false);
    ui->fire_button->setEnabled(false);
    ui->power_out_button->setEnabled(false);
    ui->help_button->setEnabled(false);
}

// re-enable buttons after sim is set up
void MainWindow::enableAllButtons()
{
    ui->sim_submit->setEnabled(true);
    ui->pass_submit->setEnabled(true);
    ui->start_sim_button->setEnabled(true);
    ui->stop_sim_button->setEnabled(true);
    ui->pause_sim_button->setEnabled(true);
    ui->cont_sim_button->setEnabled(true);
    ui->fire_button->setEnabled(true);
    ui->power_out_button->setEnabled(true);
    ui->help_button->setEnabled(true);
}

// resets input widgets to default/min values
void MainWindow::resetAllBoxes()
{
    ui->num_elev->setValue(ui->num_elev->minimum());
    ui->num_floors->setValue(ui->num_floors->minimum());
    ui->num_pass->setValue(ui->num_pass->minimum());
    ui->dest_floor_select->setValue(ui->dest_floor_select->minimum());
    ui->elev_select->setValue(ui->elev_select->minimum());
    ui->pass_select->setValue(ui->pass_select->minimum());
    ui->start_floor_select->setValue(ui->start_floor_select->minimum());
    ui->floor_timestep->setValue(ui->floor_timestep->minimum());
    ui->open_door_button->setChecked(false);
    ui->close_door_button->setChecked(false);
    ui->obstruct_door_button->setChecked(false);
}

// resets the passenger input portion specifically
void MainWindow::resetPassBoxes()
{
    ui->dest_floor_select->setValue(ui->dest_floor_select->minimum());
    ui->elev_select->setValue(ui->elev_select->minimum());
    ui->pass_select->setValue(ui->pass_select->minimum());
    ui->start_floor_select->setValue(ui->start_floor_select->minimum());
    ui->floor_timestep->setValue(ui->floor_timestep->minimum());
    ui->open_door_button->setChecked(false);
    ui->close_door_button->setChecked(false);
    ui->obstruct_door_button->setChecked(false);
}

// resets simulation setup to default values
void MainWindow::resetSimulationSetup() {
    ui->num_floors->setValue(ui->num_floors->minimum());
    ui->num_elev->setValue(ui->num_elev->minimum());
    ui->num_pass->setValue(ui->num_pass->minimum());

    //re-enable submit
    ui->sim_submit->setEnabled(true);
}

void MainWindow::resetPassengerSetup() {
    ui->pass_select->setValue(ui->pass_select->minimum());
    ui->start_floor_select->setValue(ui->start_floor_select->minimum());
    ui->dest_floor_select->setValue(ui->dest_floor_select->minimum());
    ui->floor_timestep->setValue(ui->floor_timestep->minimum());
    ui->open_door_button->setChecked(false);
    ui->close_door_button->setChecked(false);
    ui->obstruct_door_button->setChecked(false);
    ui->pass_submit->setEnabled(true);
}

// appends a line of text to the log_output text box
void MainWindow::outputLog(const std::string &output)
{
    ui->log_output->appendPlainText(QString::fromStdString(output));
}

// initializes the sim environment and UI
void MainWindow::setUpSim()
{
    connectAllSlots();
    disableAllButtons();

    // allow sim setup first
    ui->sim_submit->setEnabled(true);

    ui->log_output->setReadOnly(true);
    ui->timestep_display->setReadOnly(true);
}

// called when user submits the sim setup (#floors, #elev, #pass)
void MainWindow::submitSim()
{
    ui->sim_submit->setEnabled(false);

    num_floors = ui->num_floors->value();
    num_elev   = ui->num_elev->value();
    num_pass   = ui->num_pass->value();

    // set range for passenger + elevator selection
    ui->pass_select->setRange(1,num_pass);
    ui->start_floor_select->setRange(1,num_floors);
    ui->dest_floor_select->setRange(1,num_floors);
    ui->elev_select->setRange(1,num_elev);

    // init passenger + elevator vectors
    passengers.resize(num_pass);
    elevators.resize(num_elev);

    // example: alternate up/down for each elevator
    for(int i=0; i<num_elev; i++){
        if(i % 2 != 0){
            elevators[i].setGoingUp(false);
            elevators[i].setCurrentFloor(num_floors);
        }
    }

    // now user can specify passenger details, or start sim
    ui->pass_submit->setEnabled(true);
    ui->start_sim_button->setEnabled(true);

    updateElevatorDisplays();
}

// called when user sets a passenger's request data
void MainWindow::submitPass()
{
    bool goingUp = true;
    if(ui->start_floor_select->value() - ui->dest_floor_select->value() > 0){
        goingUp = false;
    }

    // update passenger data
    passengers[ui->pass_select->value() - 1].update(
        ui->start_floor_select->value(),
        ui->dest_floor_select->value(),
        ui->floor_timestep->value(),
        ui->start_floor_select->value(),
        ui->open_door_button->isChecked(),
        ui->close_door_button->isChecked(),
        ui->obstruct_door_button->isChecked(),
        goingUp
    );

    resetPassBoxes();
}

// user pressed "Start Simulation"
void MainWindow::startSim()
{
    enableAllButtons();
    resetAllBoxes();

    ui->pass_submit->setEnabled(false);
    ui->sim_submit->setEnabled(false);
    ui->start_sim_button->setEnabled(false);
    ui->cont_sim_button->setEnabled(false);

    ui->log_output->clear();
    outputLog("~Simulation started");
    timer.start(1500); // 1.5s per time step
}

// user pressed "Stop Simulation"
void MainWindow::stopSim()
{
    outputLog("");
    outputLog("~Simulation stopped");

    timer.stop();
    curr_ts = 0;
    ui->timestep_display->setPlainText(QString::number(curr_ts));

    // reset all elevators to floor 1 (or 0 if needed)
    for (int i = 0; i < num_elev; i++) {
        elevators[i].setCurrentFloor(1);  // set each elevator to floor 1
    }

    updateElevatorDisplays();  // ensure UI updates to reflect reset state

    // reset all data
    disconnectAllSlots();
    elevators.clear();
    passengers.clear();

    setUpSim(); // reinitialize UI state
}

// user pressed "Pause Simulation"
void MainWindow::pauseSim()
{
    outputLog("");
    outputLog("Simulation paused");
    ui->cont_sim_button->setEnabled(true);
    ui->pause_sim_button->setEnabled(false);
    timer.stop();
}

// user pressed "Continue Simulation"
void MainWindow::contSim()
{
    outputLog("Simulation continued");
    ui->cont_sim_button->setEnabled(false);
    ui->pause_sim_button->setEnabled(true);
    timer.start(1500);
}

// updates at every time step (timer callback)
void MainWindow::advanceTimestep()
{
    curr_ts++;
    ui->timestep_display->setPlainText(QString::number(curr_ts));
    outputLog(""); // blank line for readability

    getOffElevator();  // passengers get off first

    // 🔍 Check before moving elevators
    if (checkAllPassengersArrived()) {
        outputLog("All passengers have been serviced. All elevators are idle.");
        stopSim();
        return;  // stop before any movement happens
    }

    requestElevator();
    assignElevators();
    moveElevators();
}


// checks if all passengers have reached their destination
bool MainWindow::checkAllPassengersArrived()
{
    for(int i=0; i<num_pass; i++){
        if(!passengers[i].hasArrived()){
            return false;
        }
    }
    return true;
}

// tries to move each elevator up or down
void MainWindow::moveElevators()
{
    for(int i=0; i<num_elev; i++){
        if(elevators[i].getGoingUp()){
            // not at top floor yet
            if(elevators[i].getCurrentFloor() + 1 < num_floors){
                elevators[i].moveUp();
            }
            else if(elevators[i].getCurrentFloor() + 1 > num_floors){
                // if truly at the top, switch direction
                elevators[i].setGoingUp(false);
                elevators[i].moveDown();
            }
            else {
                // one step away from top
                elevators[i].moveUp();
                elevators[i].setGoingUp(false);
            }
        } else {
            // going down
            if(elevators[i].getCurrentFloor() - 1 > 1){
                elevators[i].moveDown();
            }
            else if(elevators[i].getCurrentFloor() - 1 < 1){
                // if truly at bottom, switch direction
                elevators[i].setGoingUp(true);
                elevators[i].moveUp();
            }
            else {
                elevators[i].moveDown();
                elevators[i].setGoingUp(true);
            }
        }
        outputLog("    Elevator " + std::to_string(i+1)
                  + " moved to floor " + std::to_string(elevators[i].getCurrentFloor()));
        movePassengers();
    }

    updateElevatorDisplays();
}

// updates passenger's currentFloor if they're on an elevator
void MainWindow::movePassengers()
{
    for(int i=0; i<num_pass; i++){
        if(passengers[i].getIsOnElevator()){
            int eIdx = passengers[i].getElevNum();
            passengers[i].setCurrentFloor(elevators[eIdx].getCurrentFloor());
        }
    }
}


// checks if any passenger's request is triggered at this time step
void MainWindow::requestElevator()
{
    for(int i=0; i<num_pass; i++){
        if(passengers[i].getTimestepRequest() == curr_ts &&
           passengers[i].getStartFloor() != passengers[i].getDestFloor()){
            std::string dir = passengers[i].getGoingUp() ? "up" : "down";
            outputLog("    Passenger " + std::to_string(i+1)
                      + " requested elevator on floor "
                      + std::to_string(passengers[i].getCurrentFloor())
                      + " going " + dir);
        }
    }
}

// tries to assign an elevator to each waiting passenger
void MainWindow::assignElevators()
{
    for(int i=0; i<num_pass; i++){
        if(passengers[i].getTimestepRequest() <= curr_ts
           && !passengers[i].hasArrived()
           && !passengers[i].getIsOnElevator()){
            for(int j=0; j<num_elev; j++){
                bool sameFloor = (elevators[j].getCurrentFloor() == passengers[i].getStartFloor());
                bool sameDir   = (passengers[i].getGoingUp() == elevators[j].getGoingUp());

                if(sameFloor && sameDir){
                    if(elevators[j].checkCapactiy()){
                        outputLog("    Passenger " + std::to_string(i+1)
                                  + " tried to board elevator " + std::to_string(j+1)
                                  + " but it was at capacity.");
                    } else {
                        getOnElevator(i, j);
                        break;
                    }
                }
            }
        }
    }
}

// passenger i boards elevator j
void MainWindow::getOnElevator(int pass_in, int elevator_in)
{
    if(passengers[pass_in].getObstructDoor()){
        outputLog("    Passenger " + std::to_string(pass_in+1)
                  + " obstructed door on elevator " + std::to_string(elevator_in+1));
    }

    outputLog("    Passenger " + std::to_string(pass_in+1)
              + " got on elevator " + std::to_string(elevator_in+1));
    passengers[pass_in].setIsOnElevator(true);
    passengers[pass_in].setElevNum(elevator_in);

    elevators[elevator_in].setNumPassengers(
                elevators[elevator_in].getNumPassengers() + 1);

    if(passengers[pass_in].getPressOpenDoor()){
        outputLog("    Passenger " + std::to_string(pass_in+1)
                  + " pressed open door on elevator "
                  + std::to_string(elevator_in+1));
    }
    if(passengers[pass_in].getPressCloseDoor()){
        outputLog("    Passenger " + std::to_string(pass_in+1)
                  + " pressed close door on elevator "
                  + std::to_string(elevator_in+1));
    }
}

// tries to have any passenger exit if they've reached their destFloor
void MainWindow::getOffElevator()
{
    for(int i=0; i<num_pass; i++){
        if(!passengers[i].hasArrived()
           && passengers[i].getIsOnElevator()
           && passengers[i].getDestFloor() == passengers[i].getCurrentFloor())
        {
            outputLog("    Passenger " + std::to_string(i+1)
                      + " got off elevator "
                      + std::to_string(passengers[i].getElevNum()+1)
                      + " on floor "
                      + std::to_string(passengers[i].getDestFloor()));

            int eIdx = passengers[i].getElevNum();
            elevators[eIdx].setNumPassengers(
                        elevators[eIdx].getNumPassengers() - 1);

            passengers[i].setIsOnElevator(false);
            passengers[i].setElevNum(-1);
        }
    }
}

void MainWindow::moveElevatorsDuringEmergency()
{
    bool allAtFloorOne = true;

    for (int i = 0; i < num_elev; i++) {
        if (elevators[i].getCurrentFloor() > 1) {
            outputLog(""); // add spacing for readability
            elevators[i].moveDown();
            outputLog("Elevator " + std::to_string(i + 1) +
                      " moved to floor " + std::to_string(elevators[i].getCurrentFloor()));

            if (elevators[i].getCurrentFloor() == 1) {
                outputLog("*Elevator " + std::to_string(i + 1) + " has reached floor 1.*");
            }

            allAtFloorOne = false;
        }

        // make sure elevators stay at floor 1
        if (elevators[i].getCurrentFloor() <= 1) {
            elevators[i].setCurrentFloor(1);
        }
    }

    updateElevatorDisplays();

    if (!allAtFloorOne) {
        QTimer::singleShot(1000, this, &MainWindow::moveElevatorsDuringEmergency);
    } else {
        outputLog("");  // add only **one** blank line before stopping
        outputLog("*All passengers have exited at floor 1.*");
        outputLog("~Simulation stopped");
        stopSim();
    }
}

// refresh elevator positions
void MainWindow::updateElevatorDisplays()
{
    ui->lcd_elevator1->display(num_elev >= 1 ? elevators[0].getCurrentFloor() : 0);
    ui->lcd_elevator2->display(num_elev >= 2 ? elevators[1].getCurrentFloor() : 0);
    ui->lcd_elevator3->display(num_elev >= 3 ? elevators[2].getCurrentFloor() : 0);
    ui->lcd_elevator4->display(num_elev >= 4 ? elevators[3].getCurrentFloor() : 0);
}

// user triggered fire button
void MainWindow::fire()
{
    outputLog("**Audio** Fire alarm sound triggered.");
    outputLog("*Fire! All passengers must exit at floor 1.*");

    timer.stop(); // stop normal simulation

    // mark all passengers for exit
    for (auto &passenger : passengers) {
        passenger.setDestFloor(1);
    }

    QTimer::singleShot(1000, this, &MainWindow::moveElevatorsDuringEmergency);
}

// user triggered power outagebutton
void MainWindow::powerOutage()
{
    outputLog("**Audio** Power outage sound triggered.");
    outputLog("*Power Outage! All elevators returning to floor 1.*");

    timer.stop(); // stop normal simulation

    QTimer::singleShot(1000, this, &MainWindow::moveElevatorsDuringEmergency);
}

// user triggered help button
void MainWindow::help()
{
    int elevIdx = ui->elev_select->value() - 1; // get selected elevator

    // log only once
    outputLog("**Audio** Help alarm sound triggered.");
    outputLog("");
    outputLog("    *Passenger requires help in Elevator " + std::to_string(elevIdx + 1) + "*");
    outputLog("    *Safely connected with building.*");

    timer.stop(); // stop normal simulation

    QTimer::singleShot(1000, this, &MainWindow::moveElevatorsDuringEmergency);
}

// i can't get this function below to work right now
// goal of this function was for the affected elevator to stop and return to floor 1
// other elevators should continue operating normally, servicing passengers
// instead, affected elevator continues to move up/down between floor, even surpassing original destinaton

/*
void MainWindow::help()
{
    int elevIdx = ui->elev_select->value() - 1;

    // only log the message if it's the first time
    static bool helpMessageDisplayed = false;
    if (!helpMessageDisplayed) {
        outputLog("**Mock Audio** Help alarm sound triggered.");
        outputLog("");
        outputLog("    *Passenger requires help in Elevator " + std::to_string(elevIdx + 1) + "*");
        outputLog("    *Safely connected with building.*");
        helpMessageDisplayed = true;
    }

    // move only the selected elevator towards floor 1
    if (elevators[elevIdx].getCurrentFloor() > 1) {
        QTimer::singleShot(1000, [=]() mutable {
            if (elevators[elevIdx].getCurrentFloor() > 1) {
                elevators[elevIdx].moveDown();
                outputLog("Elevator " + std::to_string(elevIdx + 1) +
                          " moved to floor " + std::to_string(elevators[elevIdx].getCurrentFloor()));
                updateElevatorDisplays();

                // continue moving until it reaches floor 1
                QTimer::singleShot(1000, this, &MainWindow::help);
            } else {
                // stop recursive calls when the elevator reaches floor 1
                outputLog("*Elevator " + std::to_string(elevIdx + 1) + " has safely arrived at floor 1.*");
                helpMessageDisplayed = false; // reset for future help calls
            }
        });
    }
}

*/
