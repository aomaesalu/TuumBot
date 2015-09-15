/**
 * Tuum
 * Robotex 2015
 * University of Tartu
 * Team:
 *   Kristjan Kanarbik
 *   Meelik Kiik
 *   Ants-Oskar Mäesalu
 *   Mart Müllerbeck
 *   Kaur Viiklaid
 *   Indrek Virro
 *   Mihkel Väljaots

 * 2-dimensional point class
 * Contributors:
 *   Ants-Oskar Mäesalu
 */

class Point2D {
private:
  double x;
  double y;
public:
  Point(const Point&);
  Point(const double&, const double&);
  void setX(const double&);
  void setY(const double&);
  double getX() const;
  double getY() const;
}
