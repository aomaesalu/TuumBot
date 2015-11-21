/**
 *  @file Checkerboard.hpp
 *  Perspective calibration application checkerboard class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 21 November 2015
 */

#include <vector>


namespace rtx {

  class Checkerboard {

    public:
      Checkerboard(const unsigned int&, const unsigned int&);

      const std::vector<std::vector<std::pair<unsigned int, unsigned int>>>& getPoints() const;
      unsigned int getWidth() const;
      unsigned int getHeight() const;
      unsigned int getSize() const;
      unsigned int getNumberOfPointsFilled() const;

      bool isEmpty() const;
      bool isFilled() const;

      void addPoint(const unsigned int&, const unsigned int&);

    private:
      std::vector<std::vector<std::pair<unsigned int, unsigned int>>> points;
      unsigned int width; // Number of squares in horisontal direction
      unsigned int height; // Number of squares in vertical direction
      unsigned int filled;

  };

}
