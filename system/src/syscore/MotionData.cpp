
#include "syscore/MotionData.hpp"

#include "tuum_localization.hpp"

namespace rtx { namespace Motion {

  void MotionData::clear() {
    baseVelocity = 30;
    posTargetSet = false;
    aimTargetSet = false;
    positionTarget = {0, 0};
    aimTarget = {0, 0};
  }


  /**
   *  Motion data setters.
   */
  void MotionData::setPosTarget(Vec2i vec) {
    positionTarget = vec;
    posTargetSet = true;
  }

  void MotionData::setAimTarget(Vec2i vec) {
    aimTarget = vec;
    aimTargetSet = true;
  }

  void MotionData::setTransformTarget(Transform t) {
    positionTarget = t.getPosition();
    std::cout << "CREATE AIM TARGET TO: " << t.getOrientation() << std::endl;
    aimTarget = positionTarget + Vec2i::fromOrientation(t.getOrientation());
    std::cout << positionTarget.toString()
      << "; " << aimTarget.toString()
      << "; AIM ORI: " << (aimTarget - positionTarget).getOrientation()
      << std::endl;
    posTargetSet = true;
    aimTargetSet = true;
  }


  /**
   *  Motion data getters.
   */
  Vec2i MotionData::getTargetPos() {
    if(posTargetSet)
      return positionTarget;
    else
      return Localization::getTransform()->getPosition();
  }

  double MotionData::getTargetOrient() {
    if(aimTargetSet)
      return (aimTarget - positionTarget).getOrientation();
    else
      return Localization::getTransform()->getOrientation();
  }

  double MotionData::getDeltaDist() {
    if(posTargetSet)
      return Localization::getTransform()->getPosition().distanceTo(positionTarget);
    else
      return 0.0;
  }

  double MotionData::getDeltaOrient() {
    if(aimTargetSet)
      return getTargetOrient() - Localization::getTransform()->o;
    else
      return 0.0;
  }

  Transform MotionData::getTargetTransform() {
    return Transform({getTargetPos(), getTargetOrient()});
  }


  /**
   *  Omni drive parameter calculation members
   */
  void MotionData::calc() {
    updateGear();

    if(posTargetSet)
      _speed = (int)(baseVelocity);
    else
      _speed = 0;

    if(aimTargetSet)
      _r_speed = (int)(baseVelocity*0.35*getOVF());
    else
      _r_speed = 0;

    //Vec2f _dV = dV;
    //_dV.rotate(-orientDelta);
    _heading = (positionTarget - Localization::getTransform()->getPosition()).getOrientation();
  }

  void MotionData::updateGear() {

  }

  void MotionData::applyFactors() {
    _speed *= getVF();
    //_r_speed *= getOVF();
  }

  void MotionData::clamp() {
    if(fabs(_r_speed) < m_rotGear->v && _r_speed != 0) {
      _r_speed = m_rotGear->v * (_r_speed < 0 ? -1 : 1);
    }
  }

  double MotionData::getVF() {
    double mag = (positionTarget - Localization::getTransform()->getPosition()).getMagnitude();

    if(mag >= VLS_DIST.low) return 1;
    return mag / VLS_DIST.low;
  }

  double MotionData::getOVF() {
    double orientDelta = (aimTarget - positionTarget).getOrientation();
    double oD = fabs(orientDelta);

    int sign = orientDelta < 0 ? -1 : 1;
    double gradient_upper_limit = 0.70;

    if(oD <= m_rotGear->step) return 0;
    if(oD > gradient_upper_limit) return sign;

    double v = (oD - m_rotGear->step) * sign / (gradient_upper_limit - m_rotGear->step);
    return v;
  }

}}
