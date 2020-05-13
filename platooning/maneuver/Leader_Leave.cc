#include "veins/modules/application/platooning/maneuver/Leader_Leave.h"
#include "veins/modules/application/platooning/apps/GeneralPlatooningApp.h"

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

        //Leader sends leader message to all
        for (unsigned int i = 1; i < positionHelper->getPlatoonSize(); i++) {
            Leave* l = createLeave(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getMemberId(i), positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), positionHelper->getPlatoonFormation());
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

        //if the message comes from the leader
        if (pb->getVehicleId() == positionHelper->getLeaderId()) {
            traciVehicle->setLeaderVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed());
        }
        //if message comes from front vehicle, in this case is the leader
        int frontId = positionHelper->getLeaderId();
        if (pb->getVehicleId() == frontId) {
            //get front vehicle position
            Coord frontPosition(pb->getPositionX(), pb->getPositionY(), 0);
            //get my position
            Veins::TraCICoord traciPosition = mobility->getManager()->omnet2traci(mobility->getCurrentPosition());
            Coord position(traciPosition.x, traciPosition.y);
            //compute distance
            double distance = position.distance(frontPosition) - pb->getLength();
            traciVehicle->setFrontVehicleFakeData(pb->getControllerAcceleration(), pb->getAcceleration(), pb->getSpeed(), distance);
            MoveToPositionAck* ack = createMoveToPositionAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId(), positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), positionHelper->getPlatoonFormation());
            app->sendUnicast(ack, positionHelper->getLeaderId());
            leaveManeuverState = LeaveManeuverState::A_WAIT_GAP;
        }
    }
}

void Leader_Leave::handleLeave(const Leave* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (app->getPlatoonRole() != PlatoonRole::AVAILABLE) return;

    /*save data from original platoon*/
    originalPlatoonData->from(msg);

    LeaveAck* lack = createLeaveResponse(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId());
    app->sendUnicast(lack, positionHelper->getLeaderId());

    leaveManeuverState = LeaveManeuverState::A_WAIT_GAP;
}

void Leader_Leave::handleLeaveAck(const LeaveAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;

    // disable lane changing during maneuver
    traciVehicle->setFixedLane(traciVehicle->getLaneIndex());
    positionHelper->setPlatoonLane(traciVehicle->getLaneIndex());

    // save some data. who is rejoining?
    reJoinerData.reset(new ReJoinerData());
    reJoinerData->from(msg);

    Gap* cgap = createGap(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getMemberId(reJoinerData->reJoinerId + 1), 12);
    app->sendUnicast(cgap, positionHelper->getMemberId(reJoinerData->reJoinerId + 1));

    leaveManeuverState = LeaveManeuverState::L_WAIT_GAP;
}

void Leader_Leave::handleGap(const Gap* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (app->getPlatoonRole() != PlatoonRole::FOLLOWER) return;
    if (leaveManeuverState != LeaveManeuverState::IDLE) return;

    //create a temp platoon to avoid crashes when deaccelerating
    std::vector<int> temp_formation;
    for (unsigned int i = positionHelper->getId(); i < positionHelper->getPlatoonSize(); i++) {
        temp_formation.push_back(positionHelper->getMemberId(i));
    }
    positionHelper->setPlatoonFormation(temp_formation);

    // activate faked CACC. this way we can approach the front car
    // using data obtained through GPS
    traciVehicle->setCACCConstantSpacing(15);
    // we have no data so far, so for the moment just initialize
    // with some fake data
    traciVehicle->setLeaderVehicleFakeData(0, 0, originalPlatoonData->platoonSpeed);
    traciVehicle->setFrontVehicleFakeData(0, 0, originalPlatoonData->platoonSpeed, 15);
    traciVehicle->setActiveController(Plexe::FAKED_CACC);

    GapAck* cgapack = createGapAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), msg->getVehicleId(), positionHelper->getPlatoonSpeed(), positionHelper->getPlatoonLane(), temp_formation);
    app->sendUnicast(cgapack, msg->getVehicleId());

    leaveManeuverState = LeaveManeuverState::F_WAIT_CLOSE;
}

void Leader_Leave::handleGapAck(const GapAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_GAP) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return; 
    
    /** talvez seja necessário criar novo pelotão**/
    
    Lane* lane = createLaneChange(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId, positionHelper->getPlatoonLane() + 1);
    app->sendUnicast(lane, reJoinerData->reJoinerId);

    leaveManeuverState = LeaveManeuverState::L_WAIT_LANE_CHANGE;

}

void Leader_Leave::handleLane(const Lane* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::A_WAIT_GAP) return;
    if (app->getPlatoonRole() != PlatoonRole::AVAILABLE) return;
    
    /* Change lane to the required one*/
    traciVehicle->setFixedLane(msg->getNewLane(), false);
    reJoinerData->reJoinerLane = msg->getNewLane();
    
    LaneAck* laneack = createLaneChangeAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId());
    app->sendUnicast(laneack, positionHelper->getLeaderId());

    leaveManeuverState = LeaveManeuverState::A_WAIT_INFORMATION;
}

void Leader_Leave::handleLaneAck(const LaneAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_LANE_CHANGE) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;

    Close* cls = createCloseGap(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId + 1);
    app->sendUnicast(cls, reJoinerData->reJoinerId + 1);

    leaveManeuverState = LeaveManeuverState::L_WAIT_GAP_CLOSE;
}

void Leader_Leave::handleClose(const Close* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::F_WAIT_CLOSE) return;
    if (app->getPlatoonRole() != PlatoonRole::FOLLOWER) return;

    /*close gap*/
    positionHelper->setFrontId(reJoinerData->reJoinerFrontId);
    traciVehicle->setFrontVehicleFakeData(0, 0, originalPlatoonData->platoonSpeed, 15);

    traciVehicle->setActiveController(Plexe::CACC);
    traciVehicle->setCACCConstantSpacing(5);

    CloseAck* clsAck = createCloseGapAck(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), positionHelper->getLeaderId());
    app->sendUnicast(clsAck, positionHelper->getLeaderId()); /** mudar este leader id se necessário novo pelotão**/

    leaveManeuverState = LeaveManeuverState::IDLE;
}

void Leader_Leave::handleCloseAck(const CloseAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_GAP_CLOSE) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;

    /*insert platoon in correct position*/
    reJoinerData->newFormation = positionHelper->getPlatoonFormation();
    reJoinerData->newFormation.erase(reJoinerData->reJoinerId);
    reJoinerData->newFormation.emplace(1, reJoinerData->reJoinerId);

    MovetoPosition* mtp = createMoveToPosition(positionHelper->getId(), positionHelper->getExternalId(), positionHelper->getPlatoonId(), reJoinerData->reJoinerId, positionHelper->getPlatoonSpeed(), originalPlatoonData->newFormation);
    app->sendUnicast(mtp, reJoinerData->reJoinerId);

    leaveManeuverState = LeaveManeuverState::L_WAIT_A_IN_POSITION;    
}

void Leader_Leave::handleMoveToPosition(const MoveToPosition* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::A_WAIT_INFORMATION) return;
    if (app->getPlatoonRole() != PlatoonRole::AVAILABLE) return;

    /*set new frontId */
    positionHelper->setFrontId(originalPlatoonData->frontId());

    // activate faked CACC. this way we can approach the front car
    // using data obtained through GPS
    traciVehicle->setCACCConstantSpacing(10);
    // we have no data so far, so for the moment just initialize
    // with some fake data
    traciVehicle->setLeaderVehicleFakeData(0, 0, targetPlatoonData->platoonSpeed);
    traciVehicle->setFrontVehicleFakeData(0, 0, targetPlatoonData->platoonSpeed, 10);
    // set a CC speed higher than the platoon speed to approach it
    traciVehicle->setCruiseControlDesiredSpeed(targetPlatoonData->platoonSpeed + (30 / 3.6));
    traciVehicle->setActiveController(Plexe::FAKED_CACC);

    leaveManeuverState = LeaveManeuverState::A_MOVE_IN_POSITION;
}

void Leader_Leave::handleMoveToPositionAck(const MoveToPositionAck* msg)
{
    if (msg->getDestinationId() != positionHelper->getId()) return;
    if (leaveManeuverState != LeaveManeuverState::L_WAIT_A_IN_POSITION) return;
    if (app->getPlatoonRole() != PlatoonRole::LEADER) return;

    leaveManeuverState = LeaveManeuverState::L_WAIT_GAP;
}

void Leader_Leave::handleJoinFormation(const JoinFormation* msg)
{
    app->setPlatoonRole(PlatoonRole::LEADER);
    leaveManeuverState = LeaveManeuverState::IDLE;
}

void Leader_Leave::handleJoinFormationAck(const JoinFormationAck* msg)
{
    app->setPlatoonRole(PlatoonRole::NONE);
    leaveManeuverState = LeaveManeuverState::IDLE;
}