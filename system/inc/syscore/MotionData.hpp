
#ifndef RTX_MOTION_DATA_H
#define RTX_MOTION_DATA_H

#include "rtxmath.hpp"

#include "motion.hpp"

namespace rtx { namespace Motion {

  struct MotionData {
    Vec2i positionTarget;
    Vec2i aimTarget;
    Vec2i _pt;
    Vec2i _at;

    bool posTargetSet = false;
    bool aimTargetSet = false;

    void clear();

    void setPosTarget(Vec2i vec);
    int setAimTarget(Vec2i vec);
    void setTransformTarget(Transform t);

    Vec2i getTargetPos();
    double getTargetOrient();

    Transform getTargetTransform();
    Vec2i getAimVector();

    double getDeltaDist();
    double getDeltaOrient();

    int getTargetRange();


    /**
     *  Omni drive parameter calculation members
     */
    int baseVelocity = 25;

    double _speed;
    double _r_speed;
    double _heading;

    const Gear* m_movGear = &(GRS_MOV.low);
    const Gear* m_rotGear = &(GRS_ROT.low);
    Gear manualRotGear = {0, 0};

    double getHeading() { return _heading; }
    int getSpeed() { return _speed; }
    int getRotationSpeed() { return _r_speed; }

    void calc();

    void updateGear();
    void applyFactors();
    void clamp();

    double getVF();
    double getOVF();

  };

}}

#endif // RTX_MOTION_DATA_H
