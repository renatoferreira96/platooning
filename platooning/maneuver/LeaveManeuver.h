#ifndef LEAVEMANEUVER_H_
#define LEAVEMANEUVER_H_

#include "veins/modules/application/platooning/maneuver/Maneuver.h"
#include "veins/modules/application/platooning/messages/MoveToPositionAck_m.h"
#include "veins/modules/application/platooning/messages/MoveToPosition_m.h"
#include "veins/modules/application/platooning/messages/ManeuverMessage_m.h"
#include "veins/modules/application/platooning/messages/JoinFormationAck_m.h"
#include "veins/modules/application/platooning/messages/JoinFormation_m.h"
#include "veins/modules/application/platooning/messages/Gap_m.h"
#include "veins/modules/application/platooning/messages/GapAck_m.h"
#include "veins/modules/application/platooning/messages/Lane_m.h"
#include "veins/modules/application/platooning/messages/LaneAck_m.h"
#include "veins/modules/application/platooning/messages/Close_m.h"
#include "veins/modules/application/platooning/messages/CloseAck_m.h"
#include "veins/modules/application/platooning/messages/Leave_m.h"
#include "veins/modules/application/platooning/messages/LeaveAck_m.h"

struct LeaveManeuverParameters {
    int platoonId;
    int leaderId;
};

class LeaveManeuver : public Maneuver {

public:
    /**
     * Constructor
     *
     * @param app pointer to the generic application used to fetch parameters and inform it about a concluded maneuver
     */
    LeaveManeuver(GeneralPlatooningApp* app);
    virtual ~LeaveManeuver(){};

    virtual void onManeuverMessage(const ManeuverMessage* mm) override;

protected:
    /**
     * Creates a Leave Request message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param double platoonSpeed the speed of the platoon
     * @param int platoonLane the id of the lane of the platoon
     * @param std::vector<int> newPlatoonFormation the original platoon
     */
    Leave* createLeave(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation);

    /**
     * Creates a Leave response message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     */
    LeaveAck* createLeaveResponse(int vehicleId, std::string externalId, int platoonId, int destinationId);

    /**
     * Creates a Close gap message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     */
    Close* createCloseGap(int vehicleId, std::string externalId, int platoonId, int destinationId);

    /**
     * Creates a Close gap ack message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param int currentLaneIndex the index of the current lane of the vehicle
     */
    CloseAck* createCloseGapAck(int vehicleId, std::string externalId, int platoonId, int destinationId);

    /**
     * Creates a Gap message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param double distance the distance of required spacing to front car
     */
    Gap* createGap(int vehicleId, std::string externalId, int platoonId, int destinationId, double distance);

    /**
     * Creates a Gap ack message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param double platoonSpeed the speed of the platoon
     * @param int platoonLane the id of the lane of the platoon
     * @param std::vector<int> newPlatoonFormation the platoon formation after
     * the join maneuver
     */
    GapAck* createGapAck(int vehicleId, std::string externalId, int platoonId, int destinationId,  double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation);

    /**
     * Creates a Lane change message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param int newLane the index of the required lane for the vehicle
     */
    Lane* createLaneChange(int vehicleId, std::string externalId, int platoonId, int destinationId, int newLane);

    /**
     * Creates a Lane ack message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     */
    LaneAck* createLaneChangeAck(int vehicleId, std::string externalId, int platoonId, int destinationId);

    /**
     * Creates a MoveToPosition message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param int platoonSpeed the speed of the platoon
     * @param int platoonLane the id of the lane of the platoon
     * @param std::vector<int> newPlatoonFormation the platoon formation after
     * the join maneuver
     */
    MoveToPosition* createMoveToPosition(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation);

    /**
     * Creates a MoveToPositionAck message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param int platoonSpeed the speed of the platoon
     * @param int platoonLane the id of the lane of the platoon
     * @param std::vector<int> newPlatoonFormation the platoon formation after
     * the join maneuver
     */
    MoveToPositionAck* createMoveToPositionAck(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation);

    /**
     * Creates a JoinFormation message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param int platoonSpeed the speed of the platoon
     * @param int platoonLane the id of the lane of the platoon
     * @param std::vector<int> newPlatoonFormation the platoon formation after
     * the join maneuver
     */
    JoinFormation* createJoinFormation(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation);

    /**
     * Creates a JoinFormationAck message
     *
     * @param int vehicleId the id of the sending vehicle
     * @param int platoonId the id of the platoon of the sending vehicle
     * @param int destinationId the id of the destination
     * @param int platoonSpeed the speed of the platoon
     * @param int platoonLane the id of the lane of the platoon
     * @param std::vector<int> newPlatoonFormation the platoon formation after
     * the join maneuver
     */
    JoinFormationAck* createJoinFormationAck(int vehicleId, std::string externalId, int platoonId, int destinationId, double platoonSpeed, int platoonLane, const std::vector<int>& newPlatoonFormation);

     /**
     * Handles a Leave msg in the context of this application
     *
     * @param Leave msg to handle
     */
    virtual void handleLeave(const Leave* msg) = 0;

     /**
     * Handles a LeaveAck msg in the context of this application
     *
     * @param LeaveAck msg to handle
     */
    virtual void handleLeaveAck(const LeaveAck* msg) = 0;

     /**
     * Handles a Close gap msg in the context of this application
     *
     * @param Close msg to handle
     */
    virtual void handleClose(const Close* msg) = 0;

     /**
     * Handles a Close ack gap msg in the context of this application
     *
     * @param CloseAck msg to handle
     */
    virtual void handleCloseAck(const CloseAck* msg) = 0;

     /**
     * Handles a Gap msg in the context of this application
     *
     * @param Gap msg to handle
     */
    virtual void handleGap(const Gap* msg) = 0;

     /**
     * Handles a Gap ack msg in the context of this application
     *
     * @param GapAck msg to handle
     */
    virtual void handleGapAck(const GapAck* msg) = 0;

     /**
     * Handles a Lane msg in the context of this application
     *
     * @param Lane msg to handle
     */
    virtual void handleLane(const Lane* msg) = 0;

     /**
     * Handles a Lane ack msg in the context of this application
     *
     * @param LaneAck msg to handle
     */
    virtual void handleLaneAck(const LaneAck* msg) = 0;

    /**
     * Handles a MoveToPosition in the context of this application
     *
     * @param MoveToPosition msg to handle
     */
    virtual void handleMoveToPosition(const MoveToPosition* msg) = 0;

    /**
     * Handles a MoveToPositionAck in the context of this application
     *
     * @param MoveToPositionAck msg to handle
     */
    virtual void handleMoveToPositionAck(const MoveToPositionAck* msg) = 0;

    /**
     * Handles a JoinFormation in the context of this application
     *
     * @param JoinFormation msg to handle
     */
    virtual void handleJoinFormation(const JoinFormation* msg) = 0;

    /**
     * Handles a JoinFormationAck in the context of this application
     *
     * @param JoinFormationAck msg to handle
     */
    virtual void handleJoinFormationAck(const JoinFormationAck* msg) = 0;
};

#endif