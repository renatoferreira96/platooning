#include "plexe/maneuver/Leader_Leave.h"
#include "plexe/apps/GeneralPlatooningApp.h"

namespace plexe{

Leader_Leave::Leader_Leave(GeneralPlatooningApp* app)
    : LeaveManeuver(app)
    , leaveManeuverState(LeaveManeuverState::IDLE)
{
}

void Leader_Leave::startManeuver(const void* parameters)
{
    LeaveManeuverParameters* pars = (LeaveManeuverParameters*) parameters;
    if(leaveManeuverState == LeaveManeuverState::IDLE) {
        ASSERT(app->getPlatoonRole() == PlatoonRole::LEADER);
        ASSERT(!app->isInManeuver());

        app->setInManeuver(true);
        //Leader sends leader message to all
        for (unsigned int i = 1; i < positionHelper->getPlatoonSize(); i++) {
            Leave* l = createLeave(positionHelper->getId(), positionHelper->getExternalId(), pars->platoonId, positionHelper->getMemberId(i), positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), positionHelper->getPlatoonFormation(), pars->leaderId);
            app->sendUnicast(l, positionHelper->getMemberId(i));
        }
        
        leaveManeuverState = LeaveManeuverState::L_WAIT_REPLY;
    }
}

void Leader_Leave::abortManeuver()
{
}

void Leader_Leave::onPlatoonBeacon(const PlatooningBeacon* pb)
{
    if (leaveManeuverState == LeaveManeuverState::A_MOVE_IN_POSITION) {
        //check correct role
        ASSERT(app->getPlatoonRole() == PlatoonRole::AVAILABLE);
        
        // if the message comes from the leader
        if (pb->getVehicleId() == targetPlatoonData->newFormation.at(0)) {
            plexeTraciVehicle->setLeaderVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed());
        }
        // if the message comes from the front vehicle
        int frontPosition = targetPlatoonData->joinIndex - 1;
        int frontId = targetPlatoonData->newFormation.at(frontPosition);
        if (pb->getVehicleId() == frontId) {
            // get front vehicle position
            Coord frontPosition(pb->getPositionX(), pb->getPositionY(), 0);
            // get my position
            veins::TraCICoord traciPosition = mobility->getManager()->getConnection()->omnet2traci(mobility->getPositionAt(simTime()));
            Coord position(traciPosition.x, traciPosition.y);
            // compute distance
            double distance = position.distance(frontPosition) - pb->getLength();
            plexeTraciVehicle->setFrontVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed(), distance);
            // if we are in position, tell the leader about that
            if (distance < 8) {
                MoveToPositionAck* ack = createMoveToPositionAck(positionHelper->getId(), positionHelper->getExternalId(), targetPlatoonData->platoonId, targetPlatoonData->platoonLeader, targetPlatoonData->platoonSpeed, targetPlatoonData->platoonLane, targetPlatoonData->newFormation);
                app->sendUnicast(ack, targetPlatoonData->newFormation.at(0));
                leaveManeuverState = LeaveManeuverState::A_WAIT_GAP;
            }
        }
    }
    else if (leaveManeuverState == LeaveManeuverState::F_WAIT_CLOSE){
        //check correct role
        ASSERT(app->getPlatoonRole() == PlatoonRole::FOLLOWER);
        int frontPosition = originalPlatoonData->joinIndex - 1;
        int frontId = originalPlatoonData->originalFormation.at(frontPosition);
        if (pb->getVehicleId() == frontId) {
            // get front vehicle position
            Coord frontPosition(pb->getPositionX(), pb->getPositionY(), 0);
            // get my position
            veins::TraCICoord traciPosition = mobility->getManager()->getConnection()->omnet2traci(mobility->getPositionAt(simTime()));
            Coord position(traciPosition.x, traciPosition.y);
            // compute distance
            double distance = position.distance(frontPosition) - pb->getLength();
            plexeTraciVehicle->setFrontVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed(), distance);
            // if we are in position, tell the leader about that
            if (distance > 12) {
                GapAck* cgapack = createGapAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), originalPlatoonData->platoonLeader, positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane());
                app->sendUnicast(cgapack, originalPlatoonData->platoonLeader);
                leaveManeuverState = LeaveManeuverState::F_WAITS_INFORMATION;
            }
        }
    }
    else if (leaveManeuverState == LeaveManeuverState::SPACING){
        ASSERT(app->getPlatoonRole() == PlatoonRole::FOLLOWER);
        int frontId = positionHelper->getFrontId();
        if (pb->getVehicleId() == frontId) {
            // get front vehicle position
            Coord frontPosition(pb->getPositionX(), pb->getPositionY(), 0);
            // get my position
            veins::TraCICoord traciPosition = mobility->getManager()->getConnection()->omnet2traci(mobility->getPositionAt(simTime()));
            Coord position(traciPosition.x, traciPosition.y);
            // compute distance
            double distance = position.distance(frontPosition) - pb->getLength();
            plexeTraciVehicle->setFrontVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed(), distance);
            // if we are in position, tell the leader about that
            if (distance > 8) {
                DissolveAck* dsack = createDissolveAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId());
                app->sendUnicast(dsack, positionHelper->getLeaderId());
                app->setPlatoonRole(PlatoonRole::NONE);
                traciVehicle->setColor(TraCIColor::TraCIColor(255, 0, 0, 255));
                plexeTraciVehicle->setFixedLane(1, false);
                plexeTraciVehicle->setCruiseControlDesiredSpeed(130.0 / 3.6);
                leaveManeuverState = LeaveManeuverState::IDLE;
            }
        }
    }

    else if (leaveManeuverState == LeaveManeuverState::A_SPACING) {
        ASSERT(app->getPlatoonRole() == PlatoonRole::AVAILABLE);
        int frontId = positionHelper->getFrontId();
        if (pb->getVehicleId() == frontId) {
            // get front vehicle position
            Coord frontPosition(pb->getPositionX(), pb->getPositionY(), 0);
            // get my position
            veins::TraCICoord traciPosition = mobility->getManager()->getConnection()->omnet2traci(mobility->getPositionAt(simTime()));
            Coord position(traciPosition.x, traciPosition.y);
            // compute distance
            double distance = position.distance(frontPosition) - pb->getLength();
            plexeTraciVehicle->setFrontVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed(), distance);
            // if we are in position, tell the leader about that
            if (distance > 8) {

                plexeTraciVehicle->setFixedLane(requiredLane, false);

                if (requiredLane == 1) plexeTraciVehicle->setActiveController(ACC);
                else plexeTraciVehicle->setActiveController(CACC);
    
                LaneAck* laneack = createLaneChangeAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId(), requiredLane);
                app->sendUnicast(laneack, positionHelper->getLeaderId());

                if(requiredLane == 1) leaveManeuverState = LeaveManeuverState::A_WAIT_INFORMATION;
                else leaveManeuverState = LeaveManeuverState::A_WAIT_REJOIN;
            }
        }
    }
}

void Leader_Leave::onFailedTransmissionAttempt(const ManeuverMessage* mm)
{
    throw cRuntimeError("Impossible to send this packet: %s. Maximum number of unicast retries reached", mm->getName());
}

void Leader_Leave::handleLeave(const Leave* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (app->getPlatoonRole() == PlatoonRole::AVAILABLE) {
        
        if (positionHelper->getId() == positionHelper->getPlatoonSize() - 1) {
            app->setInManeuver(true);

            // collect information about target Platoon
            targetPlatoonData.reset(new TargetPlatoonData());
            targetPlatoonData->platoonId = msg->getPlatoonId();
            targetPlatoonData->platoonLeader = msg->getPlatoonLeaderId();

            LeaveAck* lack = createLeaveResponse(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId(), positionHelper->getPlatoonLane() + 1, positionHelper->getFrontId());
            app->sendUnicast(lack, positionHelper->getLeaderId());

            plexeTraciVehicle->setFixedLane(positionHelper->getPlatoonLane() + 1, false);

            leaveManeuverState = LeaveManeuverState::A_WAIT_INFORMATION;
        }

        else if (positionHelper->getId() == 1){
            app->setInManeuver(true);

            // collect information about target Platoon
            targetPlatoonData.reset(new TargetPlatoonData());
            targetPlatoonData->platoonId = msg->getPlatoonId();
            targetPlatoonData->platoonLeader = msg->getPlatoonLeaderId();

            LeaveAck* lack = createLeaveResponse(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId(), positionHelper->getPlatoonLane() + 1, positionHelper->getFrontId());
            app->sendUnicast(lack, positionHelper->getLeaderId());

            leaveManeuverState = LeaveManeuverState::A_WAIT_REJOIN;
        }
        
        else {
            app->setInManeuver(true);

            // collect information about target Platoon
            targetPlatoonData.reset(new TargetPlatoonData());
            targetPlatoonData->platoonId = msg->getPlatoonId();
            targetPlatoonData->platoonLeader = msg->getPlatoonLeaderId();

            LeaveAck* lack = createLeaveResponse(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId(), positionHelper->getPlatoonLane(), positionHelper->getFrontId());
            app->sendUnicast(lack, positionHelper->getLeaderId());

            leaveManeuverState = LeaveManeuverState::A_WAIT_GAP;
        }
        traciVehicle->setColor(TraCIColor::TraCIColor(0, 255, 0, 255));
    }
    else if (app->getPlatoonRole() == PlatoonRole::FOLLOWER) {
        static int counter = 0;

        if (positionHelper->getId() == (positionHelper->getPlatoonSize() - (positionHelper->getPlatoonSize() - positionHelper->getId())) && counter == (positionHelper->getId() - 1)) counter = positionHelper->getId();

        if (counter == positionHelper->getPlatoonSize() - 1){
            Abort* abrt = createAbort(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId(), true);
            app->sendUnicast(abrt, positionHelper->getLeaderId());
        }
    }
}

void Leader_Leave::handleAbort(const Abort* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;

    if (msg->getAbortFlag() == true) {
        if (positionHelper->getPlatoonSize() > 1) {
            Dissolve* ds = createDissolve(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getMemberId(positionHelper->getPlatoonSize() - 1));
            app->sendUnicast(ds, positionHelper->getMemberId(positionHelper->getPlatoonSize() - 1));
        }
    }

    leaveManeuverState = LeaveManeuverState::ABORTING;
}

void Leader_Leave::handleDissolve(const Dissolve* msg)
{   
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (app->getPlatoonRole() != PlatoonRole::FOLLOWER) return;

    
    if (positionHelper->getId() == positionHelper->getMemberId(positionHelper->getPlatoonSize() - 1)) {
        plexeTraciVehicle->setCACCConstantSpacing(10.0);
        leaveManeuverState = LeaveManeuverState::SPACING;
    }
}

void Leader_Leave::handleDissolveAck(const DissolveAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;
    if (leaveManeuverState != LeaveManeuverState::ABORTING) return;

    std::vector<int> diss_formation;
    diss_formation = positionHelper->getPlatoonFormation();
    diss_formation.erase(diss_formation.begin() + msg->getVehicleId());
    positionHelper->setPlatoonFormation(diss_formation);
    
    // send to all vehicles in Platoon
    for (unsigned int i = 1; i < diss_formation.size(); i++) {
        UpdatePlatoonFormation* dup = app->createUpdatePlatoonFormation(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), -1, positionHelper->getPlatoonSpeed(), traciVehicle->getLaneIndex(), diss_formation);
        int dest = diss_formation.at(i);
        dup->setDestinationId(dest);
        app->sendUnicast(dup, dest);
    }

    if (positionHelper->getPlatoonSize() > 1) {
        Dissolve* ds = createDissolve(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getMemberId(positionHelper->getPlatoonSize() - 1));
        app->sendUnicast(ds, positionHelper->getMemberId(positionHelper->getPlatoonSize() - 1));
    }
    else
    {
        app->setPlatoonRole(PlatoonRole::NONE);
        traciVehicle->setColor(TraCIColor::TraCIColor(255, 0, 0, 255));
        plexeTraciVehicle->setFixedLane(positionHelper->getPlatoonLane(), false);
        plexeTraciVehicle->setCruiseControlDesiredSpeed(100.0 / 3.6);
        leaveManeuverState = LeaveManeuverState::IDLE;
    }
}

void Leader_Leave::handleLeaveAck(const LeaveAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;

    // disable lane changing during maneuver
    plexeTraciVehicle->setFixedLane(traciVehicle->getLaneIndex());
    positionHelper->setPlatoonLane(traciVehicle->getLaneIndex());

    // save some data. who is rejoining?
    reJoinerData.reset(new ReJoinerData());
    reJoinerData->from(msg);    

    if (msg->getVehicleId() == positionHelper->getMemberId(positionHelper->getPlatoonSize() - 1)){
        
        std::vector<int> formation;
        formation = positionHelper->getPlatoonFormation();
        formation.erase(formation.begin() + reJoinerData->reJoinerId);
        positionHelper->setPlatoonFormation(formation);

        
        reJoinerData->newFormation = positionHelper->getPlatoonFormation();
        reJoinerData->newFormation.emplace(reJoinerData->newFormation.begin() + 1, reJoinerData->reJoinerId);
        
        MoveToPosition* mtp = createMoveToPosition(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId, positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), reJoinerData->newFormation);
        app->sendUnicast(mtp, reJoinerData->reJoinerId);

        leaveManeuverState = LeaveManeuverState::L_WAIT_A_IN_POSITION;
    } 
    else if (msg->getVehicleId() == 1) {
        reJoinerData->newFormation = positionHelper->getPlatoonFormation();
        JoinFormation* jf = createJoinFormation(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId, positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), reJoinerData->newFormation);
        app->sendUnicast(jf, reJoinerData->reJoinerId);
        leaveManeuverState = LeaveManeuverState::L_WAIT_A_TO_REJOIN;
    }
    else {
        Gap* cgap = createGap(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getMemberId(reJoinerData->reJoinerId + 1), positionHelper->getPlatoonSpeed(), 15, positionHelper->getPlatoonFormation(), reJoinerData->reJoinerId);
        app->sendUnicast(cgap, positionHelper->getMemberId(reJoinerData->reJoinerId + 1));

        leaveManeuverState = LeaveManeuverState::L_WAIT_GAP;
    }
}

void Leader_Leave::handleGap(const Gap* msg)
{
    if (positionHelper->getId() != msg->getDestinationId()) return;
    if (app->getPlatoonRole() != PlatoonRole::FOLLOWER) return;
    if (leaveManeuverState != LeaveManeuverState::IDLE) return;

    app->setInManeuver(true);

    originalPlatoonData.reset(new OriginalPlatoonData());
    originalPlatoonData->from(msg);

    std::vector<int> temp_formation;
    if (positionHelper->getId() == 1) {
        //create a temp platoon to avoid crashes when deaccelerating
        for (unsigned int i = positionHelper->getId(); i < positionHelper->getPlatoonSize(); i++) {
            temp_formation.push_back(positionHelper->getMemberId(i));
        }
        temp_formation.erase(temp_formation.begin() + msg->getReJoinerId() - 1);
        positionHelper->setPlatoonFormation(temp_formation);
    }
    
    else {
        //create a temp platoon to avoid crashes when deaccelerating
        
        for (unsigned int i = positionHelper->getId(); i < positionHelper->getPlatoonSize(); i++) {
            temp_formation.push_back(positionHelper->getMemberId(i));
        }
        positionHelper->setPlatoonFormation(temp_formation);
    }

    
    if (temp_formation.size() > 1) {    
        for (unsigned int i = 1; i < temp_formation.size(); i++) {
            UpdatePlatoonFormation* dup = app->createUpdatePlatoonFormation(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), -1, positionHelper->getPlatoonSpeed(), traciVehicle->getLaneIndex(), temp_formation);
            int dest = temp_formation.at(i);
            dup->setDestinationId(dest);
            app->sendUnicast(dup, dest);
        }
    }

    plexeTraciVehicle->setCACCConstantSpacing(msg->getDistance());
 
    // we have no data so far, so for the moment just initialize
    // with some fake data
    plexeTraciVehicle->setLeaderVehicleFakeData(0, 0, msg->getSpeed()); 
    plexeTraciVehicle->setFrontVehicleFakeData(0, 0, msg->getSpeed(), msg->getDistance());
    plexeTraciVehicle->setActiveController(plexe::FAKED_CACC);

    leaveManeuverState = LeaveManeuverState::F_WAIT_CLOSE;
}

void Leader_Leave::handleGapAck(const GapAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_GAP) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return; 
    

    if (reJoinerData->reJoinerLane == positionHelper->getPlatoonLane()) {
        Lane* lane = createLaneChange(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId, positionHelper->getPlatoonLane() + 1);
        app->sendUnicast(lane, reJoinerData->reJoinerId);
    }
    if (reJoinerData->reJoinerLane > positionHelper->getPlatoonLane()){
        Lane* lane = createLaneChange(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId, positionHelper->getPlatoonLane());
        app->sendUnicast(lane, reJoinerData->reJoinerId);
    }

    leaveManeuverState = LeaveManeuverState::L_WAIT_LANE_CHANGE;
}

void Leader_Leave::handleLane(const Lane* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::A_WAIT_GAP) return;
    if (app->getPlatoonRole() != PlatoonRole::AVAILABLE) return;
    
    requiredLane = msg->getNewLane();
    if (requiredLane > 0) plexeTraciVehicle->setCACCConstantSpacing(12);
        
    leaveManeuverState = LeaveManeuverState::A_SPACING;
}

void Leader_Leave::handleLaneAck(const LaneAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_LANE_CHANGE) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;

    reJoinerData->reJoinerLane = msg->getLaneAfterAck();

    std::vector<int> formation;
    formation = positionHelper->getPlatoonFormation();
    if (reJoinerData->reJoinerLane != positionHelper->getPlatoonLane()) {
        formation.erase(formation.begin() + reJoinerData->reJoinerId);
        positionHelper->setPlatoonFormation(formation);
    }

    if (msg->getLaneAfterAck() == 1) { 
        Close* cls = createCloseGap(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId + 1, positionHelper->getPlatoonSpeed());
        app->sendUnicast(cls, reJoinerData->reJoinerId + 1);
    }
    else if (msg->getLaneAfterAck() == 0) {
        Close* cls = createCloseGap(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->newFormation.at(2), positionHelper->getPlatoonSpeed());
        app->sendUnicast(cls, reJoinerData->newFormation.at(2));
    }

    leaveManeuverState = LeaveManeuverState::L_WAIT_GAP_CLOSE;
}

void Leader_Leave::handleClose(const Close* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::F_WAITS_INFORMATION) return;
    if (app->getPlatoonRole() != PlatoonRole::FOLLOWER) return;

    plexeTraciVehicle->setCACCConstantSpacing(5);
    plexeTraciVehicle->setLeaderVehicleFakeData(0, 0, msg->getSpeed());
    plexeTraciVehicle->setFrontVehicleFakeData(0, 0, msg->getSpeed(), 5);
    plexeTraciVehicle->setActiveController(plexe::CACC);
    

    if (positionHelper->getId() == 1){
        CloseAck* clsAck = createCloseGapAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), msg->getVehicleId(), 0);
        app->sendUnicast(clsAck, msg->getVehicleId());
    }
    else {
        CloseAck* clsAck = createCloseGapAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), msg->getVehicleId(), 1);
        app->sendUnicast(clsAck, msg->getVehicleId());
    }
    app->setInManeuver(false);
    leaveManeuverState = LeaveManeuverState::IDLE;
}

void Leader_Leave::handleCloseAck(const CloseAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_GAP_CLOSE) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;
    
    if (msg->getSequenceNumber() == 1) {
        /*insert rejoiner in correct position*/
        reJoinerData->newFormation = positionHelper->getPlatoonFormation();
        reJoinerData->newFormation.emplace(reJoinerData->newFormation.begin() + 1, reJoinerData->reJoinerId);
        
        MoveToPosition* mtp = createMoveToPosition(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId, positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), reJoinerData->newFormation);
        app->sendUnicast(mtp, reJoinerData->reJoinerId);

        leaveManeuverState = LeaveManeuverState::L_WAIT_A_IN_POSITION; 
    }
    else if (msg->getSequenceNumber() == 0) {
        
        JoinFormation* jf = createJoinFormation(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId, positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), reJoinerData->newFormation);
        app->sendUnicast(jf, reJoinerData->reJoinerId);
        leaveManeuverState = LeaveManeuverState::L_WAIT_A_TO_REJOIN;
    }  
}

void Leader_Leave::handleMoveToPosition(const MoveToPosition* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::A_WAIT_INFORMATION) return;
    if (app->getPlatoonRole() != PlatoonRole::AVAILABLE) return;

    //save information about the platoon
    targetPlatoonData->from(msg);

    // activate faked CACC. this way we can approach the front car
    // using data obtained through GPS
    plexeTraciVehicle->setCACCConstantSpacing(5);
    // we have no data so far, so for the moment just initialize
    // with some fake data
    plexeTraciVehicle->setLeaderVehicleFakeData(0, 0, targetPlatoonData->platoonSpeed);
    plexeTraciVehicle->setFrontVehicleFakeData(0, 0, targetPlatoonData->platoonSpeed, 5);
    // set a CC speed higher than the platoon speed to approach it

    plexeTraciVehicle->setCruiseControlDesiredSpeed(targetPlatoonData->platoonSpeed + (30 / 3.6));
    plexeTraciVehicle->setActiveController(plexe::FAKED_CACC);

    leaveManeuverState = LeaveManeuverState::A_MOVE_IN_POSITION;
}

void Leader_Leave::handleMoveToPositionAck(const MoveToPositionAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_A_IN_POSITION) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;

    for (unsigned i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) ASSERT(msg->getNewPlatoonFormation(i) == reJoinerData->newFormation.at(i));

    Gap* cgap = createGap(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->newFormation.at(2), positionHelper->getPlatoonSpeed(), 15, positionHelper->getPlatoonFormation(), reJoinerData->reJoinerId);
    app->sendUnicast(cgap, reJoinerData->newFormation.at(2));

    leaveManeuverState = LeaveManeuverState::L_WAIT_GAP;
}

void Leader_Leave::handleJoinFormation(const JoinFormation* msg)
{
    if (app->getPlatoonRole() != PlatoonRole::AVAILABLE) return;
    if (leaveManeuverState != LeaveManeuverState::A_WAIT_REJOIN) return;
    if (msg->getPlatoonId() != targetPlatoonData->platoonId) return;
    if (msg->getVehicleId() != targetPlatoonData->platoonLeader) return;
    
    for (unsigned i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) ASSERT(msg->getNewPlatoonFormation(i) == targetPlatoonData->newFormation[i]);
    // we got confirmation from the leader
    // switch from faked CACC to real CACC
    plexeTraciVehicle->setActiveController(ACC);
    plexeTraciVehicle->setCruiseControlDesiredSpeed(100.0 / 3.6);
    
    // update platoon information
    positionHelper->setPlatoonId(msg->getPlatoonId());
    positionHelper->setPlatoonLane(targetPlatoonData->platoonLane);
    positionHelper->setPlatoonSpeed(targetPlatoonData->platoonSpeed);
    std::vector<int> formation;
    for (unsigned i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) formation.push_back(msg->getNewPlatoonFormation(i));
    positionHelper->setPlatoonFormation(formation);

    // tell the leader that we're now in the platoon
    JoinFormationAck* jfa = createJoinFormationAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), targetPlatoonData->platoonLeader, positionHelper->getPlatoonSpeed(), traciVehicle->getLaneIndex(), formation);
    app->sendUnicast(jfa, targetPlatoonData->platoonLeader);
    
    app->setPlatoonRole(PlatoonRole::LEADER);
    leaveManeuverState = LeaveManeuverState::IDLE;
    traciVehicle->setColor(TraCIColor::TraCIColor(0, 0, 255, 255));
}

void Leader_Leave::handleJoinFormationAck(const JoinFormationAck* msg)
{
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_A_TO_REJOIN) return;
    if (msg->getVehicleId() != reJoinerData->reJoinerId) return;
    
    for (unsigned i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) ASSERT(msg->getNewPlatoonFormation(i) == reJoinerData->newFormation.at(i));
    
    // the joiner has joined the platoon
    // add the joiner to the list of vehicles in the platoon
    std::vector<int> final_form;
    for (unsigned int i = 1 ; i < reJoinerData->newFormation.size(); i++) {
        final_form.push_back(reJoinerData->newFormation.at(i));
    }
    positionHelper->setPlatoonFormation(final_form);

    // send to all vehicles in Platoon
    for (unsigned int i = 1; i < final_form.size(); i++) {
        UpdatePlatoonFormation* dup = app->createUpdatePlatoonFormation(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), -1, positionHelper->getPlatoonSpeed(), traciVehicle->getLaneIndex(), final_form);
        int dest = final_form.at(i);
        dup->setDestinationId(dest);
        app->sendUnicast(dup, dest);
    }
    
    app->setPlatoonRole(PlatoonRole::NONE);
    leaveManeuverState = LeaveManeuverState::IDLE;
    traciVehicle->setColor(TraCIColor::TraCIColor(255, 0, 0, 255));
    
    app->setInManeuver(false);
    plexeTraciVehicle->setFixedLane(1, false);
    plexeTraciVehicle->setCruiseControlDesiredSpeed(130.0 / 3.6);
}

} // namespace plexe