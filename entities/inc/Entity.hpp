/**
 * @file Entity.hpp
 * Entity class.
 *
 * @authors Ants-Oskar Mäesalu
 * @version 0.1
 */

#ifndef RTX_ENTITIES_ENTITY_H
#define RTX_ENTITIES_ENTITY_H

#include "Point2D.hpp"


namespace rtx {

  class Entity {
    public:
      Entity(const Entity&);
      Entity(const Point2D*);
      Entity(const double&, const double&);

      void setPosition(const Point2D*);
      void setPosition(const double&, const double&);

      Point2D* getPosition() const;

    private:
      Point2D *position;
  };

};

#endif // RTX_ENTITIES_ENTITY_H
