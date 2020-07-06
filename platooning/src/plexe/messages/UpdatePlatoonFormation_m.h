//
// Generated file, do not edit! Created by nedtool 5.5 from plexe/messages/UpdatePlatoonFormation.msg.
//

#ifndef __UPDATEPLATOONFORMATION_M_H
#define __UPDATEPLATOONFORMATION_M_H

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
 * Class generated from <tt>plexe/messages/UpdatePlatoonFormation.msg:30</tt> by nedtool.
 * <pre>
 * // Message to inform the all vehicles in the Platoon of the updated formation.
 * // Is similar to a PlatoonBeacon.
 * packet UpdatePlatoonFormation extends ManeuverMessage
 * {
 *     double platoonSpeed;
 *     int platoonLane;
 *     int platoonFormation[];
 * }
 * </pre>
 */
class PLEXE_API UpdatePlatoonFormation : public ::ManeuverMessage
{
  protected:
    double platoonSpeed;
    int platoonLane;
    int *platoonFormation; // array ptr
    unsigned int platoonFormation_arraysize;

  private:
    void copy(const UpdatePlatoonFormation& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const UpdatePlatoonFormation&);

  public:
    UpdatePlatoonFormation(const char *name=nullptr, short kind=0);
    UpdatePlatoonFormation(const UpdatePlatoonFormation& other);
    virtual ~UpdatePlatoonFormation();
    UpdatePlatoonFormation& operator=(const UpdatePlatoonFormation& other);
    virtual UpdatePlatoonFormation *dup() const override {return new UpdatePlatoonFormation(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual double getPlatoonSpeed() const;
    virtual void setPlatoonSpeed(double platoonSpeed);
    virtual int getPlatoonLane() const;
    virtual void setPlatoonLane(int platoonLane);
    virtual void setPlatoonFormationArraySize(unsigned int size);
    virtual unsigned int getPlatoonFormationArraySize() const;
    virtual int getPlatoonFormation(unsigned int k) const;
    virtual void setPlatoonFormation(unsigned int k, int platoonFormation);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const UpdatePlatoonFormation& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, UpdatePlatoonFormation& obj) {obj.parsimUnpack(b);}


#endif // ifndef __UPDATEPLATOONFORMATION_M_H

