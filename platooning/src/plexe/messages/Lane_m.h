//
// Generated file, do not edit! Created by nedtool 5.5 from plexe/messages/Lane.msg.
//

#ifndef __LANE_M_H
#define __LANE_M_H

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
 * Class generated from <tt>plexe/messages/Lane.msg:24</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet Lane extends ManeuverMessage
 * {
 *     int newLane;
 * }
 * </pre>
 */
class PLEXE_API Lane : public ::ManeuverMessage
{
  protected:
    int newLane;

  private:
    void copy(const Lane& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const Lane&);

  public:
    Lane(const char *name=nullptr, short kind=0);
    Lane(const Lane& other);
    virtual ~Lane();
    Lane& operator=(const Lane& other);
    virtual Lane *dup() const override {return new Lane(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getNewLane() const;
    virtual void setNewLane(int newLane);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const Lane& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, Lane& obj) {obj.parsimUnpack(b);}


#endif // ifndef __LANE_M_H

