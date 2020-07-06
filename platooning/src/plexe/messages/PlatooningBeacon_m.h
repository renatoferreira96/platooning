//
// Generated file, do not edit! Created by nedtool 5.5 from plexe/messages/PlatooningBeacon.msg.
//

#ifndef __PLATOONINGBEACON_M_H
#define __PLATOONINGBEACON_M_H

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



/**
 * Class generated from <tt>plexe/messages/PlatooningBeacon.msg:21</tt> by nedtool.
 * <pre>
 * packet PlatooningBeacon
 * {
 *     //id of the originator
 *     int vehicleId = 0;
 *     double controllerAcceleration = 0;
 *     double acceleration = 0;
 *     double speed = 0;
 *     double positionX = 0;
 *     double positionY = 0;
 *     double time = 0;
 *     int sequenceNumber = 0;
 *     double length = 0;
 *     double speedX = 0;
 *     double speedY = 0;
 *     double angle = 0;
 * }
 * </pre>
 */
class PLEXE_API PlatooningBeacon : public ::omnetpp::cPacket
{
  protected:
    int vehicleId;
    double controllerAcceleration;
    double acceleration;
    double speed;
    double positionX;
    double positionY;
    double time;
    int sequenceNumber;
    double length;
    double speedX;
    double speedY;
    double angle;

  private:
    void copy(const PlatooningBeacon& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const PlatooningBeacon&);

  public:
    PlatooningBeacon(const char *name=nullptr, short kind=0);
    PlatooningBeacon(const PlatooningBeacon& other);
    virtual ~PlatooningBeacon();
    PlatooningBeacon& operator=(const PlatooningBeacon& other);
    virtual PlatooningBeacon *dup() const override {return new PlatooningBeacon(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getVehicleId() const;
    virtual void setVehicleId(int vehicleId);
    virtual double getControllerAcceleration() const;
    virtual void setControllerAcceleration(double controllerAcceleration);
    virtual double getAcceleration() const;
    virtual void setAcceleration(double acceleration);
    virtual double getSpeed() const;
    virtual void setSpeed(double speed);
    virtual double getPositionX() const;
    virtual void setPositionX(double positionX);
    virtual double getPositionY() const;
    virtual void setPositionY(double positionY);
    virtual double getTime() const;
    virtual void setTime(double time);
    virtual int getSequenceNumber() const;
    virtual void setSequenceNumber(int sequenceNumber);
    virtual double getLength() const;
    virtual void setLength(double length);
    virtual double getSpeedX() const;
    virtual void setSpeedX(double speedX);
    virtual double getSpeedY() const;
    virtual void setSpeedY(double speedY);
    virtual double getAngle() const;
    virtual void setAngle(double angle);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const PlatooningBeacon& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, PlatooningBeacon& obj) {obj.parsimUnpack(b);}


#endif // ifndef __PLATOONINGBEACON_M_H

