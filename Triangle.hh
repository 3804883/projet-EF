#ifndef DEF_TRIANGLE
#define DEF_TRIANGLE

#include "Point.hh"
#include <cmath>
#include <vector>

class Triangle
{

  friend class Point;

protected:
  //variables:
  Point *s1;
  Point *s2;
  Point *s3;
  std::vector< std::pair<Point*, Point*> > listEdge;
  int label;
  double surface;

public:

  //methods:

  void __init__(Point *m_s1, Point *m_s2, Point *m_s3, int m_label);

  //return a pointer over the point of index i in the traingle
  inline Point* getPoint(int index) const{
    if (index == 0) {
      return s1;
    }
    if (index == 1) {
      return s2;
    }
    if (index == 2) {
      return s3;
    }
    else{
      std::cerr << "Index out of range, please pick a number from 0 to 2" << '\n';
    }
  };

  //return a pointer over the points of the triangle
  inline Point** getPoints() const{

    std::vector<Point*> P(3);

    P.push_back(s1);
    P.push_back(s2);
    P.push_back(s3);

    return P.data();
  };

  //return the label of the triangle
  inline int getLabel() const{
    return label;
  };

  //return the surface of the triangle
  inline double getSurface() const{
    if(surface != 0){
      return surface;
    }
    else{
      std::cout << "Set the surface first" << '\n';
      return 0;
    }
  };

  //return a pointer over the list of the edges of the triangle
  inline std::pair<Point*, Point*>* getListEdge(){
    return listEdge.data();
  };

  //set the surface of the triangle
  void setSurface();
  void setEdge();

};

//overloads:
/*friend std::ostream& operator<<(std::ostream &os, const Triangle &T)
{
  os << T.getPoint(0)->getLabel() << '\n' << T.getPoint(1)->getLabel() << '\n' << T.getPoint(2)->getLabel() << '\n' << T.getLabel() << " " << std::endl;
  return os;
}*/


#endif
