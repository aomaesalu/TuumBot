
#include "syscore/MotionData.hpp"

#include "tuum_localization.hpp"

namespace rtx { namespace Motion {

  void MotionData::clear() {
    baseVelocity = 30;
    posTargetSet = false;
    aimTargetSet = false;
    manualRotGear = {0, 0};
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

  int MotionData::setAimTarget(Vec2i vec) {
    /*double mag = fabs((vec - _pt).getMagnitude());
    if(mag < GRS_ROT.mn.step + mag*434/5000) return -1;
    _pt = vec;
    */

    aimTarget = vec;
    aimTargetSet = true;
    return 0;
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

  Vec2i MotionData::getAimVector() {
    return aimTarget - positionTarget;
  }


  /**
   *  Omni drive parameter calculation members
   */
  void MotionData::calc() {
    updateGear();

    if(posTargetSet)
      _speed = baseVelocity;
    else
      _speed = 0;

    if(aimTargetSet)
      _r_speed = m_rotGear->v;
    else
      _r_speed = 0;

    //Vec2f _dV = dV;
    //_dV.rotate(-orientDelta);
    _heading = (positionTarget - Localization::getTransform()->getPosition()).getOrientation();
  }

  void MotionData::updateGear() {
    double d;
    bool rot_gear_unset = true,  mov_gear_unset = true;

    if(posTargetSet) {
      d = fabs(getDeltaDist());

      if(d < VLS_DIST.mn) m_movGear = &(GRS_MOV.mn);
      else if(d < VLS_DIST.low) m_movGear = &(GRS_MOV.low);
      else if(d < VLS_DIST.med) m_movGear = &(GRS_MOV.med);
      else if(d < VLS_DIST.high) m_movGear = &(GRS_MOV.high);
      else {
	m_movGear = &(GRS_MOV.mx);
	m_rotGear = &(GRS_ROT.mx);
	rot_gear_unset = false;
      }

      baseVelocity = m_movGear->v;
      mov_gear_unset = false;
    }

    if(rot_gear_unset && manualRotGear.v != 0) {
      m_rotGear = &(manualRotGear);
    } else {
      if(aimTargetSet && rot_gear_unset) {
	d = fabs(getDeltaOrient());
	if(d < VLS_ANGLE.low) m_rotGear = &(GRS_ROT.low);
	else if(d < VLS_ANGLE.med) m_rotGear = &(GRS_ROT.med);
	else if(d < VLS_ANGLE.high) m_rotGear = &(GRS_ROT.high);
	else m_rotGear = &(GRS_ROT.mn);
      }
    }
  }

  void MotionData::applyFactors() {
    _speed *= getVF();
    if(m_rotGear != &(manualRotGear)) _r_speed *= getOVF();
  }

  void MotionData::clamp() {
    if(fabs(_r_speed) < 5 && _r_speed != 0) {
      _r_speed = 5 * (_r_speed < 0 ? -1 : 1);
    }
  }

  double MotionData::getVF() {
    double mag = (positionTarget - Localization::getTransform()->getPosition()).getMagnitude();

    if(mag >= VLS_DIST.low) return 1;
    return mag / VLS_DIST.mn;
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
