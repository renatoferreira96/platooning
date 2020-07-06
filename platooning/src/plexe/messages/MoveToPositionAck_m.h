//
// Generated file, do not edit! Created by nedtool 5.5 from plexe/messages/MoveToPositionAck.msg.
//

#ifndef __MOVETOPOSITIONACK_M_H
#define __MOVETOPOSITIONACK_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0505
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif

// dll export symbol
#ifndef PLEXE_API
#  if defined(PLEXE_EXPORT)
#    define PLEXE_API  OPP_DLLEXPORT
#  elif defined(PLEXE_IMPORT)
#    define PLEXE_API  OPP_DLLIMPORT
#  else
#    define PLEXE_API
#  endif
#endif



// cplusplus {{
#include "ManeuverMessage_m.h"
// }}

/**
 * Class generated from <tt>plexe/messages/MoveToPositionAck.msg:32</tt> by nedtool.
 * <pre>
 * // Response to a MoveToPosition.
 * // Is sent from an accepted joiner to the leader of the Platoon.
 * // Confirms the successful reception of the Platoon information and the position
 * // to join.
 * packet MoveToPositionAck extends ManeuverMessage
 * {
 *     double platoonSpeed;
 *     int platoonLane;
 *     int newPlatoonFormation[];
 * }
 * </pre>
 */
class PLEXE_API MoveToPositionAck : public ::ManeuverMessage
{
  protected:
    double platoonSpeed;
    int platoonLane;
    int *newPlatoonFormation; // array ptr
    unsigned int newPlatoonFormation_arraysize;

  private:
    void copy(const MoveToPositionAck& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const MoveToPositionAck&);

  public:
    MoveToPositionAck(const char *name=nullptr, short kind=0);
    MoveToPositionAck(const MoveToPositionAck& other);
    virtual ~MoveToPositionAck();
    MoveToPositionAck& operator=(const MoveToPositionAck& other);
    virtual MoveToPositionAck *dup() const override {return new MoveToPositionAck(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual double getPlatoonSpeed() const;
    virtual void setPlatoonSpeed(double platoonSpeed);
    virtual int getPlatoonLane() const;
    virtual void setPlatoonLane(int platoonLane);
    virtual void setNewPlatoonFormationArraySize(unsigned int size);
    virtual unsigned int getNewPlatoonFormationArraySize() const;
    virtual int getNewPlatoonFormation(unsigned int k) const;
    virtual void setNewPlatoonFormation(unsigned int k, int newPlatoonFormation);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const MoveToPositionAck& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, MoveToPositionAck& obj) {obj.parsimUnpack(b);}


#endif // ifndef __MOVETOPOSITIONACK_M_H

