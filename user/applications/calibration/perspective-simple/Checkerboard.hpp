/**
 *  @file Checkerboard.hpp
 *  Perspective calibration application checkerboard class.
 *
 *  @authors Ants-Oskar Mäesalu
 *  @version 0.1
 *  @date 24 November 2015
 */

#include <vector>


namespace rtx {

  class Checkerboard {

    public:
      Checkerboard(const unsigned int&, const unsigned int&, const unsigned int&);

      const std::vector<std::vector<std::pair<unsigned int, unsigned int>>>& getPoints() const;
      unsigned int getWidth() const;
      unsigned int getHeight() const;
      unsigned int getSize() const;
      unsigned int getSquareWidth() const;
      unsigned int getNumberOfPointsFilled() const;

      double getA() const;
      double getB() const;
      double getC() const;

      bool isEmpty() const;
      bool isFilled() const;

      void addPoint(const unsigned int&, const unsigned int&);

      void calculateConstants();

      std::pair<double, double> virtualToReal(const unsigned int&, const unsigned int&);
      std::pair<unsigned int, unsigned int> realToVirtual(const double&, const double&);

    private:
      std::vector<std::vector<std::pair<unsigned int, unsigned int>>> points;
      unsigned int width; // Number of squares in horisontal direction
      unsigned int height; // Number of squares in vertical direction
      unsigned int squareWidth; // Square width in the real world (in mm)
      unsigned int filled;

      // Perspective constants
      double A;
      double B;
      double C;

  };

}
