#include "plexe/maneuver/LeaveManeuver.h"
#include "plexe/apps/GeneralPlatooningApp.h"

namespace plexe {

LeaveManeuver::LeaveManeuver(GeneralPlatooningApp* app)
    :Maneuver(app)
{
}

void LeaveManeuver::onManeuverMessage(const ManeuverMessage* mm)
{
    if (const Leave* msg = dynamic_cast<const Leave*>(mm)) {
        handleLeave(msg);
    }
    else if (const LeaveAck* msg = dynamic_cast<const LeaveAck*>(mm)) {
        handleLeaveAck(msg);
    }
    else if (const Abort* msg = dynamic_cast<const Abort*>(mm)) {
        handleAbort(msg);
    }
    else if (const Dissolve* msg = dynamic_cast<const Dissolve*>(mm)) {
        handleDissolve(msg);
    }
    else if (const DissolveAck* msg = dynamic_cast<const DissolveAck*>(mm)) {
        handleDissolveAck(msg);
    }
    else if (const Close* msg = dynamic_cast<const Close*>(mm)) {
        handleClose(msg);
    }
    else if (const CloseAck* msg = dynamic_cast<const CloseAck*>(mm)) {
        handleCloseAck(msg);
    }
    else if (const Gap* msg = dynamic_cast<const Gap*> (mm)) {
        handleGap(msg);
    }
    else if (const GapAck* msg = dynamic_cast<const GapAck*> (mm)) {
        handleGapAck(msg);
    }
    else if (const Lane* msg = dynamic_cast<const Lane*> (mm)) {
        handleLane(msg);
    }
    else if (const LaneAck* msg = dynamic_cast<const LaneAck*> (mm)) {
        handleLaneAck(msg);
    }
    else if (const MoveToPosition* msg = dynamic_cast<const MoveToPosition*>(mm)) {
        handleMoveToPosition(msg);
    }
    else if (const MoveToPositionAck* msg = dynamic_cast<const MoveToPositionAck*>(mm)) {
        handleMoveToPositionAck(msg);
    }
    else if (const JoinFormation* msg = dynamic_cast<const JoinFormation*>(mm)) {
        handleJoinFormation(msg);
    }
    else if (const JoinFormationAck* msg = dynamic_cast<const JoinFormationAck*>(mm)) {
        handleJoinFormationAck(msg);
    }
}

Leave* LeaveManeuver::createLeave(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation, int platoonLeaderId)
{
    Leave* msg = new Leave("Leave");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setPlatoonSpeed(platoonSpeed);
    msg->setPlatoonLane(platoonLane);
    msg->setNewPlatoonFormationArraySize(newPlatoonFormation.size());
    for (unsigned int i = 0; i < newPlatoonFormation.size(); i++) {
        msg->setNewPlatoonFormation(i, newPlatoonFormation[i]);
    }
    msg->setPlatoonLeaderId(platoonLeaderId);
    return msg;
}

LeaveAck* LeaveManeuver::createLeaveResponse(int vehicleId, std::string externalId, int platoonId, int destinationId, int platoonLane, int frontId)
{
    LeaveAck* msg = new LeaveAck("LeaveAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setCurrentLaneIndex(platoonLane);
    msg->setFrontId(frontId);
    return msg;
}

Abort* LeaveManeuver::createAbort(int vehicleId, std::string externalId, int platoonId, int destinationId, bool abortFlag)
{
    Abort* msg = new Abort("Abort");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setAbortFlag(abortFlag);
    return msg;
}

Close* LeaveManeuver::createCloseGap(int vehicleId, std::string externalId, int platoonId, int destinationId, double speed)
{
    Close* msg = new Close("Close");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setSpeed(speed);
    return msg;
}

CloseAck* LeaveManeuver::createCloseGapAck(int vehicleId, std::string externalId, int platoonId, int destinationId, int sequenceNum)
{
    CloseAck* msg = new CloseAck("CloseAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setSequenceNumber(sequenceNum);
    return msg;
}

Gap* LeaveManeuver::createGap(int vehicleId, std::string externalId, int platoonId, int destinationId, double speed, double distance, const std::vector<int>& originalPlatoonFormation, int reJoinerId)
{
    Gap* msg = new Gap("Gap");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setSpeed(speed);
    msg->setDistance(distance);
    msg->setOriginalPlatoonFormationArraySize(originalPlatoonFormation.size());
    for (unsigned int i = 0; i < originalPlatoonFormation.size(); i++) {
        msg->setOriginalPlatoonFormation(i, originalPlatoonFormation[i]);
    }
    msg->setReJoinerId(reJoinerId);
    return msg;
}

GapAck* LeaveManeuver::createGapAck(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane)
{
    GapAck* msg = new GapAck("GapAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setPlatoonSpeed(platoonSpeed);
    msg->setPlatoonLane(platoonLane);
    return msg;
}

Lane* LeaveManeuver::createLaneChange(int vehicleId, std::string externalId, int platoonId, int destinationId, int newLane)
{
    Lane* msg = new Lane("Lane");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setNewLane(newLane);
    return msg;
}

LaneAck* LeaveManeuver::createLaneChangeAck(int vehicleId, std::string externalId, int platoonId, int destinationId, int laneAfterAck)
{
    LaneAck* msg = new LaneAck("LaneAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setLaneAfterAck(laneAfterAck);
    return msg;
}

MoveToPosition* LeaveManeuver::createMoveToPosition(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation)
{
    MoveToPosition* msg = new MoveToPosition("MoveToPosition");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setPlatoonSpeed(platoonSpeed);
    msg->setPlatoonLane(platoonLane);
    msg->setNewPlatoonFormationArraySize(newPlatoonFormation.size());
    for (unsigned int i = 0; i < newPlatoonFormation.size(); i++) {
        msg->setNewPlatoonFormation(i, newPlatoonFormation[i]);
    }
    return msg;
}

MoveToPositionAck* LeaveManeuver::createMoveToPositionAck(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation)
{
    MoveToPositionAck* msg = new MoveToPositionAck("MoveToPositionAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setPlatoonSpeed(platoonSpeed);
    msg->setPlatoonLane(platoonLane);
    msg->setNewPlatoonFormationArraySize(newPlatoonFormation.size());
    for (unsigned int i = 0; i < newPlatoonFormation.size(); i++) {
        msg->setNewPlatoonFormation(i, newPlatoonFormation[i]);
    }
    return msg;
}

JoinFormation* LeaveManeuver::createJoinFormation(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation)
{
    JoinFormation* msg = new JoinFormation("JoinFormation");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setPlatoonSpeed(platoonSpeed);
    msg->setPlatoonLane(platoonLane);
    msg->setNewPlatoonFormationArraySize(newPlatoonFormation.size());
    for (unsigned int i = 0; i < newPlatoonFormation.size(); i++) {
        msg->setNewPlatoonFormation(i, newPlatoonFormation[i]);
    }
    return msg;
}

JoinFormationAck* LeaveManeuver::createJoinFormationAck(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation)
{
    JoinFormationAck* msg = new JoinFormationAck("JoinFormationAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setPlatoonSpeed(platoonSpeed);
    msg->setPlatoonLane(platoonLane);
    msg->setNewPlatoonFormationArraySize(newPlatoonFormation.size());
    for (unsigned int i = 0; i < newPlatoonFormation.size(); i++) {
        msg->setNewPlatoonFormation(i, newPlatoonFormation[i]);
    }
    return msg;
}

Dissolve* LeaveManeuver::createDissolve(int vehicleId, std::string externalId, int platoonId, int destinationId)
{
    Dissolve* msg = new Dissolve("Dissolve");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    return msg;
}

DissolveAck* LeaveManeuver::createDissolveAck(int vehicleId, std::string externalId, int platoonId, int destinationId)
{
    DissolveAck* msg = new DissolveAck("DissolveAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    return msg;
}

} //namespace plexe