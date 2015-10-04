/**
 * @file Entity.hpp
 * Description placeholder.
 *
 * @authors
 * @version 0.1
 */

#ifndef ENTITY_H
#define ENTITY_H

#include "Point2D.hpp"

namespace rtx {

  class Entity {
    private:
      Point2D* position;

    public:
      Entity(const Entity&);
      Entity(const Point2D*);
      Entity(const double&, const double&);

      void setPosition(const Point2D*);
      void setPosition(const double&, const double&);

      Point2D* getPosition() const;
  };

};

#endif // ENTITY_H
