#include "Point.hh"

using namespace std;

//constructors:
Point::Point(double m_x, double m_y, double m_z, int m_label) : x(m_x),
  y(m_y), z(m_z), label(m_label) {}

Point::Point(double m_x, double m_y, double m_z, int m_idFreedom,
   int m_label) : x(m_x),
     y(m_y), z(m_z), idFreedom(m_idFreedom), label(m_label) {

     }
