/**
 *  @file Corner.hpp
 *  Corner seen in the camera frame.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 29 November 2015
 */

#ifndef RTX_VISION_CORNER_H
#define RTX_VISION_CORNER_H


namespace rtx {

  class Corner {

    public:
      Corner(const Corner&);
      Corner(const double&, const double&);
      Corner(const std::pair<double, double>&);

      double getDistance() const;
      double getAngle() const;
      std::pair<double, double> getRelativePoint() const;

      void setDistance(const double&);
      void setAngle(const double&);

    private:
      double distance;
      double angle;

  };

}

#endif // RTX_VISION_CORNER_H
