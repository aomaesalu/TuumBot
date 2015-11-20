/** @file Entity.hpp
 *  Entity class.
 *
 *  @authors Ants-Oskar Mäesalu, Meelik Kiik
 *  @version 0.2
 */

#ifndef RTX_ENTITY_H
#define RTX_ENTITY_H

#include "rtxmath.hpp"

namespace rtx {

  /**
   *  class KalmanFilterObject
   *    KalmanState m_kfState[2]; // Last, current states
   *    ...?
   *
   *    void kPredict() // Velocity projection
   *    void kSense(Transform newMeasurement) // Velocity change from measurements
   *    void kEvaluate() // Updates m_kfState
   *    Transform kalmanProcess() // runs filter methods and returns probable new Transform
   *
   *  class Entity : KalmanFilterObject
   *    update(Transform transform):
   *      transform = this->kalmanProcess(Transform)
   *      ...
   *    
   */

  class Entity {
  private:
    Transform m_transform;

    int m_health = 0;

  public:
    Entity();
    Entity(const Entity&);

    // By position
    Entity(const Vec2i);
    Entity(const int, const int);

    // By position & orientation
    Entity(Transform);
    Entity(const int, const int, const double);

    int getHealth();

    Transform* getTransform();

    void update(Transform); // Heal
    void update(); // Decay
  };

};

#endif // RTX_ENTITY_H
