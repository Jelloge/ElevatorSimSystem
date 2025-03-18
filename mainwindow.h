#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <vector>
#include <string>
#include "Passenger.h"
#include "Elevator.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // simulation parameters
    int num_floors;
    int num_elev;
    int num_pass;
    int curr_ts;      // current time step

    QTimer timer;     // updates simulation every X ms

    // domain objects
    std::vector<Passenger> passengers;
    std::vector<Elevator> elevators;

private slots:
    // setup & teardown
    void connectAllSlots();
    void disconnectAllSlots();
    void disableAllButtons();
    void enableAllButtons();
    void resetAllBoxes();
    void resetPassBoxes();
    void resetSimulationSetup();
    void resetPassengerSetup();

    // logging & init
    void outputLog(const std::string &output);
    void setUpSim();

    // config / passenger input
    void submitSim();
    void submitPass();

    // sim controls
    void startSim();
    void stopSim();
    void pauseSim();
    void contSim();

    // main sim logic
    void advanceTimestep();
    bool checkAllPassengersArrived();
    void moveElevators();
    void movePassengers();
    void requestElevator();
    void assignElevators();
    void getOnElevator(int pass_in, int elevator_in);
    void getOffElevator();
    void updateElevatorDisplays();
    void moveElevatorsDuringEmergency();

    // safety mock-audio events
    void fire();
    void powerOutage();
    void help();
};

#endif // MAINWINDOW_H
