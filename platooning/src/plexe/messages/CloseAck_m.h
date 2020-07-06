//
// Generated file, do not edit! Created by nedtool 5.5 from plexe/messages/CloseAck.msg.
//

#ifndef __CLOSEACK_M_H
#define __CLOSEACK_M_H

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
 * Class generated from <tt>plexe/messages/CloseAck.msg:24</tt> by nedtool.
 * <pre>
 * //
 * // TODO generated message class
 * //
 * packet CloseAck extends ManeuverMessage
 * {
 *     int sequenceNumber;
 * }
 * </pre>
 */
class PLEXE_API CloseAck : public ::ManeuverMessage
{
  protected:
    int sequenceNumber;

  private:
    void copy(const CloseAck& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const CloseAck&);

  public:
    CloseAck(const char *name=nullptr, short kind=0);
    CloseAck(const CloseAck& other);
    virtual ~CloseAck();
    CloseAck& operator=(const CloseAck& other);
    virtual CloseAck *dup() const override {return new CloseAck(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSequenceNumber() const;
    virtual void setSequenceNumber(int sequenceNumber);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const CloseAck& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, CloseAck& obj) {obj.parsimUnpack(b);}


#endif // ifndef __CLOSEACK_M_H

