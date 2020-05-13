#include "veins/modules/application/platooning/maneuver/LeaveManeuver.h"
#include "veins/modules/application/platooning/apps/GeneralPlatooningApp.h"

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

Leave* LeaveManeuver::createLeave(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation)
{
    Leave* msg = new Leave("Leave");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setPlatoonSpeed(platoonSpeed);
    msg->setPlatoonLane(platoonLane);
    msg->setNewPlatoonFormationArraySize(newPlatoonFormation.size());
    for (unsigned int i = 0; i < newPlatoonFormation.size(); i++) {
        msg->setNewPlatoonFormation(i, newPlatoonFormation[i]);
    }
    return msg;
}

LeaveAck* LeaveManeuver::createLeaveResponse(int vehicleId, std::string externalId, int platoonId, int destinationId)
{
    LeaveAck* msg = new LeaveAck("LeaveAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    return msg;
}

Close* LeaveManeuver::createCloseGap(int vehicleId, std::string externalId, int platoonId, int destinationId)
{
    Close* msg = new Close("Close");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    return msg;
}

CloseAck* LeaveManeuver::createCloseGapAck(int vehicleId, std::string externalId, int platoonId, int destinationId)
{
    CloseAck* msg = new CloseAck("CloseAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    return msg;
}

Gap* LeaveManeuver::createGap(int vehicleId, std::string externalId, int platoonId, int destinationId, double distance)
{
    Gap* msg = new Gap("Gap");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setDistance(distance);
    return msg;
}

GapAck* LeaveManeuver::createGapAck(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation)
{
    GapAck* msg = new GapAck("GapAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setPlatoonSpeed(platoonSpeed);
    msg->setPlatoonLane(platoonLane);
    msg->setNewPlatoonFormationArraySize(newPlatoonFormation.size());
    for (unsigned int i = 0; i < newPlatoonFormation.size(); i++) {
        msg->setNewPlatoonFormation(i, newPlatoonFormation[i]);
    }
    return msg;
}

Lane* LeaveManeuver::createLaneChange(int vehicleId, std::string externalId, int platoonId, int destinationId, int newLane)
{
    Lane* msg = new Lane("Lane");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
    msg->setNewLane(newLane);
    return msg;
}

LaneAck* LeaveManeuver::createLaneChangeAck(int vehicleId, std::string externalId, int platoonId, int destinationId)
{
    LaneAck* msg = new LaneAck("LaneAck");
    app->fillManeuverMessage(msg, vehicleId, externalId, platoonId, destinationId);
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