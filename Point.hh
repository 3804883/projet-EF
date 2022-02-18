#ifndef DEF_POINT
#define DEF_POINT

#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <cmath>
#include <vector>

class Point
{
protected:
  //variables:
  double x;
  double y;
  double z;
  int idFreedom; //freedom degree index : -1 on the edge, -2 dirichlet hot, -3 dirichlet cold
  int label; //current index in the mesh

public:
  //constructors:
  Point(double m_x, double m_y, double m_z, int m_label);
  Point(double m_x, double m_y, double m_z, int m_idFreedom, int m_label);

  //methods:

  //get the coordinates of the point
  inline double* getCoord() const{
    double coord[3] = {x, y, z};
    double *p = coord;
    return p;
  };

  //get the index of global freedom degree of the point
  inline int getIdFreedom() const{
    return idFreedom;
  };

  //return the label of the point
  inline int getLabel() const{
    return label;
  };

  //return true if the point is on an edge
  inline bool IsEdge() const{

    bool Bo;
    Bo = (this->getIdFreedom() == -1) ? 1:0;
    return Bo;
  };

};

//overloads:
/*std::ostream& operator<<(std::ostream &os, const Point &P)
{
  double x, y, z ;
  x = P.getCoord()[0];
  y = P.getCoord()[1];
  z = P.getCoord()[2];

  if(P.getLabel()){
    os << x << ' ' << y << ' ' << z << ' ' << P.getLabel() << std::endl;
  }

  if(P.getLabel() && P.getIdFreedom()){
  os << x << ' ' << y << ' ' << z << ' ' << P.getLabel() << ' ' << P.getIdFreedom() << std::endl;
  }

  else{
    os << x << ' ' << y << ' ' << z << std::endl;
  }
  return os;
}*/


#endif
