#include "plexe/scenarios/LeaveManeuverScenario.h"

namespace plexe {

Define_Module(LeaveManeuverScenario);

void LeaveManeuverScenario::initialize(int stage)
{

    BaseScenario::initialize(stage);

    if(stage == 1) {
        app = FindModule<GeneralPlatooningApp*>::findSubModule(getParentModule());
        prepareManeuverCars(0);
    }
}

void LeaveManeuverScenario::setupFormation()
{
    std::vector<int> formation;
    for (int i = 0; i < 5; i++) formation.push_back(i);
    positionHelper->setPlatoonFormation(formation);
}

void LeaveManeuverScenario::prepareManeuverCars(int platoonLane)
{
    switch (positionHelper->getId()) {

    case 0: {
        //this is the leader
        plexeTraciVehicle->setCruiseControlDesiredSpeed(100.0 / 3.6);
        plexeTraciVehicle->setActiveController(ACC);
        plexeTraciVehicle->setFixedLane(platoonLane);
        app->setPlatoonRole(PlatoonRole::LEADER);
        traciVehicle->setColor(TraCIColor::TraCIColor(0, 0, 255, 255));

        // after 30 seconds of simulation, start the maneuver
        startManeuver = new cMessage();
        scheduleAt(simTime() + SimTime(10), startManeuver);
        break;
    }

    //change this setup dependent on which scenario you want to study
    //and in which position you want Available to be
    case 1:
    //case 2:
    case 3: 
    case 4:{
        // these are the followers which are already in the platoon
        plexeTraciVehicle->setCruiseControlDesiredSpeed(130.0 / 3.6);
        plexeTraciVehicle->setActiveController(CACC);
        plexeTraciVehicle->setFixedLane(platoonLane);
        app->setPlatoonRole(PlatoonRole::FOLLOWER);
        traciVehicle->setColor(TraCIColor::TraCIColor(255, 255, 255, 255));
        break;
    }
    
    case 2: {
        //this is the available car for new leader
        plexeTraciVehicle->setCruiseControlDesiredSpeed(130.0 / 3.6);
        plexeTraciVehicle->setActiveController(CACC);
        plexeTraciVehicle->setFixedLane(platoonLane);
        app->setPlatoonRole(PlatoonRole::AVAILABLE);
        traciVehicle->setColor(TraCIColor::TraCIColor(255, 255, 255, 255));
        break;
    }
    }
}

LeaveManeuverScenario::~LeaveManeuverScenario()
{
    cancelAndDelete(startManeuver);
    startManeuver = nullptr;
}

void LeaveManeuverScenario::handleSelfMsg(cMessage* msg)
{
    // this takes car of feeding data into CACC and reschedule the self message
    BaseScenario::handleSelfMsg(msg);

    if (msg == startManeuver) app->startLeaveManeuver(0, 0);
}

} // namespace plexe
