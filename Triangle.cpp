#include "Triangle.hh"

using namespace std;

//methods:

void Triangle::__init__(Point *m_s1, Point *m_s2,
  Point *m_s3, int m_label){
    s1 = m_s1;
    s2 = m_s2;
    s3 = m_s3;
    label = m_label;
  }

//setting function for the triangle 's surface'
void Triangle::setSurface(){

  surface = 1/2 * (s1->getCoord()[0] - s3->getCoord()[0])*(s2->getCoord()[1] - s3->getCoord()[1])
    - (s1->getCoord()[1] - s3->getCoord()[1])*(s2->getCoord()[0] - s3->getCoord()[0]);
  if (surface < 0) {
    surface = -surface; //using to avoid abs() which round to roughly
  }
}

//setting function for the edges
void Triangle::setEdge(){
  pair<Point*, Point*> tmp;
  tmp.first = s1;
  tmp.second = s2;
  listEdge.push_back(tmp);
  tmp.first = s1;
  tmp.second = s3;
  listEdge.push_back(tmp);
  tmp.first = s2;
  tmp.second = s3;
  listEdge.push_back(tmp);
}
