#ifndef LEADER_LEAVE_H
#define LEADER_LEAVE_H

#include <algorithm>

#include "veins/modules/application/platooning/maneuver/LeaveManeuver.h"
#include "veins/modules/application/platooning/utilities/BasePositionHelper.h"

#include "veins/modules/mobility/traci/TraCIConstants.h"
#include "veins/base/utils/Coord.h"

using namespace Veins;

class Leader_Leave : public LeaveManeuver {

public:
    /**
     * Constructor
     *
     * @param app pointer to the generic application used to fetch parameters and inform it about a concluded maneuver
     */
    Leader_Leave(GeneralPlatooningApp* app);
    virtual ~Leader_Leave(){};

    /**
     * This method is invoked by the generic application to start the maneuver
     *
     * @param parameters parameters passed to the maneuver
     */
    virtual void startManeuver(const void* parameters) override;

    /**
     * Handles the abortion of the maneuver when required by the generic application.
     * This method does currently nothing and it is meant for future used and improved maneuvers.
     */
    virtual void abortManeuver() override;

    /**
     * This method is invoked by the generic application when a beacon message is received
     * The maneuver must not free the memory of the message, as this might be needed by other maneuvers as well.
     */
    virtual void onPlatoonBeacon(const PlatooningBeacon* pb) override;

    /**
     * Handles a MoveToPosition in the context of this application
     *
     * @param MoveToPosition msg to handle
     */
    virtual void handleMoveToPosition(const MoveToPosition* msg) override;

    /**
     * Handles a MoveToPositionAck in the context of this application
     *
     * @param MoveToPositionAck msg to handle
     */
    virtual void handleMoveToPositionAck(const MoveToPositionAck* msg) override;

    /**
     * Handles a JoinFormation in the context of this application
     *
     * @param JoinFormation msg to handle
     */
    virtual void handleJoinFormation(const JoinFormation* msg) override;

    /**
     * Handles a JoinFormationAck in the context of this application
     *
     * @param JoinFormationAck msg to handle
     */
    virtual void handleJoinFormationAck(const JoinFormationAck* msg) override;

    /**
     * Handles a Gap in the context of this application
     *
     * @param Gap msg to handle
     */
    virtual void handleGap(const Gap* msg) override;

    /**
     * Handles a GapAck in the context of this application
     *
     * @param GapAck msg to handle
     */
    virtual void handleGapAck(const GapAck* msg) override;

    /**
     * Handles a Lane in the context of this application
     *
     * @param Lane msg to handle
     */
    virtual void handleLane(const Lane* msg) override;

    /**
     * Handles a Lane in the context of this application
     *
     * @param LaneAck msg to handle
     */
    virtual void handleLaneAck(const LaneAck* msg) override;

    /**
     * Handles a Leave in the context of this application
     *
     * @param Leave msg to handle
     */
    virtual void handleLeave(const Leave* msg) override;

    /**
     * Handles a LeaveAck in the context of this application
     *
     * @param LeaveAck msg to handle
     */
    virtual void handleLeaveAck(const LeaveAck* msg) override;

    /**
     * Handles a Close in the context of this application
     *
     * @param Close msg to handle
     */
    virtual void handleClose(const Close* msg) override;

    /**
     * Handles a CloseAck in the context of this application
     *
     * @param CloseAck msg to handle
     */
    virtual void handleCloseAck(const CloseAck* msg) override;

protected:
    /** Possible states a vehicle can be in during Leader leaving maneuver */
    enum class LeaveManeuverState{
        IDLE, ///< The maneuver did not start
        //Leader
        L_WAIT_REPLY, ///< The leader sent the leave msg and waits for reply
        L_WAIT_GAP, ///< The leader sent a gap msg and waits for the car to be in position
        L_WAIT_LANE_CHANGE, ///< The leader waits for A to change lane
        L_WAIT_GAP_CLOSE, ///< The leader waits for gap close
        L_WAIT_A_IN_POSITION, ///< The leader waits for A in postion
        L_WAIT_A_TO_REJOIN, ///< The leader waits for A to rejoin the platoon
        //FOLLOWER
        F_WAIT_CLOSE, ///< The follower send GapAck message and waits for Close message
        //AVAILABLE
        A_WAIT_GAP, ///< The available vehicle sends leader leave response and waits for the gap
        A_WAIT_INFORMATION, ///< The available vehicle waits for information about the Platoon
        A_MOVE_IN_POSITION, ///< The available vehicle moves to position
        A_WAIT_REJOIN, ///< The available vehicle waits for rejoin permission
    };

    /** data that a joiner stores about a Platoon it wants to join */
    struct OriginalPlatoonData {
        int platoonId; ///< the id of the platoon to join
        int platoonLeader; ///< the if ot the leader of the platoon
        int platoonLane; ///< the lane the platoon is driving on
        double platoonSpeed; ///< the speed of the platoon
        std::vector<int> newFormation; ///< the new formation of the platoon
        Coord lastFrontPos; ///< the last kwown position of the front vehicle

        /** c'tor for TargetPlatoonData */
        OriginalPlatoonData()
        {
            platoonId = INVALID_PLATOON_ID;
            platoonLeader = INVALID_INT_VALUE;
            platoonLane = INVALID_INT_VALUE;
            platoonSpeed = INVALID_DOUBLE_VALUE;
        }

        /**
         * Initializes the TargetPlatoonData object with the contents of a
         * Leave
         *
         * @param Leave msg to initialize from
         * @see Leave
         */
        void from(const Leave* msg)
        {
            platoonId = msg->getPlatoonId();
            platoonLeader = msg->getVehicleId();
            platoonLane = msg->getPlatoonLane();
            platoonSpeed = msg->getPlatoonSpeed();
            newFormation.resize(msg->getNewPlatoonFormationArraySize());
            for (unsigned int i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) {
                newFormation[i] = msg->getNewPlatoonFormation(i);
            }
        }

        /**
         * Get the id of the front vehicle
         *
         * @return int the id of the front vehicle
         */
        int frontId() const
        {
            return newFormation.at(0);
        }
    };

    /** data that a leader stores about rejoining vehicle */
    struct ReJoinerData {
        int reJoinerId; ///< the id of the vehicle joining the Platoon
        int reJoinerLane; ///< the lane chosen for joining the Platoon
        int reJoinerFrontId; ///< the id of rejoiner front vehicle
        std::vector<int> newFormation;

        /** c'tor for ReJoinerData */
        ReJoinerData()
        {
            reJoinerId = INVALID_INT_VALUE;
            reJoinerLane = INVALID_INT_VALUE;
        }

        /**
         * Initializes the JoinerData object with the contents of a
         * LeaveAck message
         *
         * @param LeaveAck  to initialize from
         * @see LeaveAck
         */
        void from(const LeaveAck* msg)
        {
            reJoinerId = msg->getVehicleId();
            reJoinerLane = msg->getCurrentLaneIndex();
            reJoinerFrontId = msg->getFrontId();
        }
    };

    /** data that the LEADER stores about a temporary platoon */
    /*struct TemporaryPlatoonData {
        int platoonId; ///< the id of the platoon to join
        int platoonLeader; ///< the if ot the leader of the platoon
        int platoonLane; ///< the lane the platoon is driving on
        double platoonSpeed; ///< the speed of the platoon
        std::vector<int> newFormation; ///< the new formation of the platoon
    
        /** c'tor for temporary platoon */
        /*TemporaryPlatoonData()
        {
            platoonId = INVALID_PLATOON_ID;
            platoonLeader = INVALID_INT_VALUE;
            platoonLane = INVALID_INT_VALUE;
            platoonSpeed = INVALID_DOUBLE_VALUE;
        }

        /**
         * Initializes the temporary platoon object with the contents of a
         * 
         *
         * @param GapAck msg to initialize from
         * @see GapAck
         */
        /*void from(const GapAck* msg)
        {
            platoonId = msg->getPlatoonId();
            platoonLeader = msg->getVehicleId();
            platoonLane = msg->getPlatoonLane();
            platoonSpeed = msg->getPlatoonSpeed();
            newFormation.resize(msg->getNewPlatoonFormationArraySize());
            for (unsigned int i = 0; i < msg->getNewPlatoonFormationArraySize(); i++) {
                newFormation[i] = msg->getNewPlatoonFormation(i);
            }
        }

        
        /**
         * Get the id of the front vehicle
         *
         * @return int the id of the front vehicle
         *//*
        int frontId() const
        {
            return newFormation.at(joinIndex - 1);
        }*/
    //};*/
    

    /** the current state in the join maneuver */
    LeaveManeuverState leaveManeuverState;

    /** the data about the reJoiner */
    std::unique_ptr<ReJoinerData> reJoinerData;

    /** the data of the platoon to rejoin */
    std::unique_ptr<OriginalPlatoonData> originalPlatoonData;

    /** the data of the temporary platoon*/
    //std::unique_ptr<TemporaryPlatoonData> temporaryPlatoonData;
    
};

#endif