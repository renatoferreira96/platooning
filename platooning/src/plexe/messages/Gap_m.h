//
// Generated file, do not edit! Created by nedtool 5.5 from plexe/messages/Gap.msg.
//

#ifndef __GAP_M_H
#define __GAP_M_H

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
 * Class generated from <tt>plexe/messages/Gap.msg:24</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet Gap extends ManeuverMessage
 * {
 *     double speed;
 *     double distance;
 *     int originalPlatoonFormation[];
 *     int reJoinerId;
 * }
 * </pre>
 */
class PLEXE_API Gap : public ::ManeuverMessage
{
  protected:
    double speed;
    double distance;
    int *originalPlatoonFormation; // array ptr
    unsigned int originalPlatoonFormation_arraysize;
    int reJoinerId;

  private:
    void copy(const Gap& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Gap&);

  public:
    Gap(const char *name=nullptr, short kind=0);
    Gap(const Gap& other);
    virtual ~Gap();
    Gap& operator=(const Gap& other);
    virtual Gap *dup() const override {return new Gap(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual double getSpeed() const;
    virtual void setSpeed(double speed);
    virtual double getDistance() const;
    virtual void setDistance(double distance);
    virtual void setOriginalPlatoonFormationArraySize(unsigned int size);
    virtual unsigned int getOriginalPlatoonFormationArraySize() const;
    virtual int getOriginalPlatoonFormation(unsigned int k) const;
    virtual void setOriginalPlatoonFormation(unsigned int k, int originalPlatoonFormation);
    virtual int getReJoinerId() const;
    virtual void setReJoinerId(int reJoinerId);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Gap& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Gap& obj) {obj.parsimUnpack(b);}


#endif // ifndef __GAP_M_H

