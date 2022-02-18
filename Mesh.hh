#ifndef DEF_MESH
#define DEF_MESH

#include "Triangle.hh"
#include <cmath>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>

class Mesh
{

  friend class Triangle;

protected:
  //variables:
  std::vector<Triangle*> listTriangle;
  std::vector<Point*> listPoint;

  Eigen::SparseMatrix<double> *SpA;
  Eigen::VectorXd *F;
  Eigen::VectorXd *X;

public:

  Mesh();
  ~Mesh();
  //methods:
  inline void totexport(){
    for (size_t i = 0; i < listPoint.size(); i++) {
      double a = listPoint[i]->getCoord()[0];
      double b = listPoint[i]->getCoord()[0];
      double c = listPoint[i]->getCoord()[0];
      std::cout << a <<' '<<b<<' '<<c<< '\n';
      double val = 0;
      if (listPoint[i]->getIdFreedom() != -1) {
        val = (*X)(listPoint[i]->getIdFreedom());

      }
      std::cout << val << '\n';
    }
    std::cout << "norm = " << (*X).norm()<< '\n';
  };
  //init the mesh by readind the .msh
  void __init__(std::string pathdir);
  //call a solver from Eigen to solve AX=B
  void solve();
  //compute the quadratik error of the computation
  double Error(double (&exactSol)(double x, double y),
        std::pair<double, double> (&grad_phi)(Triangle *T, int i));
  //Assemble the matrix A and the vector F of the problem
  void assemble(double (&f_prob)(double *p1), double (&phi)(double *p2, Triangle *T, int i),
    std::pair<double, double> (&grad_f)(double *p1), std::pair<double, double> (&grad_phi)(Triangle *T, int i));
  //get a point by its index
  inline Point* getPointM(int index) const{

    if (!listPoint.empty()) {
      if(index > listPoint.size()){
        std::cerr << "Index out of range, please pick up an index between 0 and"
          << listPoint.size() <<'\n';
      }
      else{
        return listPoint.at(index);
      }
    }
    else{
      std::cerr << "the vector is empty" << '\n';
    }
  };
  //get the list of the points in the mesh
  inline Point** getListPoint(){

    if (listPoint.empty()) {
      std::cerr << "the vector is empty" << '\n';
    }
    else{
      return listPoint.data();
    }
  };
  //get the number of triangle in the mesh
  inline int getNumberOfTriangle() const{
    if (listTriangle.empty()) {
      std::cout << "the map of triangle is empty" << '\n';
      return 0;
    }
    else{
      return listTriangle.size();
    }
  };

  //get a triangle by its index in the mesh
  inline Triangle* getTriangle(int index){
    return listTriangle[index];
  };
  //compute the barycentre of an edge
  inline double* getBarycentre(std::pair<Point*, Point*> edge) const{

    double x = (edge.first->getCoord()[0] + edge.second->getCoord()[0]) / 2;
    double y = (edge.first->getCoord()[1] + edge.second->getCoord()[1]) / 2;
    double z = (edge.first->getCoord()[2] + edge.second->getCoord()[2]) / 2;

    double tab[3] = {x, y, z};
    double *p = tab;

    return p;
  };

};

#endif
