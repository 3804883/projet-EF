#include "Mesh.hh"
#include <string>
#include <iostream>
#include <vector>

double exactSol(double x, double y){
  return sin(M_PI * x) * sin(M_PI * y);
}
double exactSol2(double x, double y){
  return exp(-x-y);
}

double f_prob(double *p1){
  return 2 * M_PI * M_PI * sin(M_PI * p1[0]) * sin(M_PI * p1[1]);
}

double f_prob2(double *p1){
  return -2 * exp(-p1[0]-p1[1]);
}

std::pair<double, double> grad_f(double *p1){
  std::pair<double, double> res;
  res.first = M_PI * cos(M_PI * p1[0]) * sin(M_PI * p1[1]);
  res.second = M_PI * sin(M_PI * p1[0]) * cos(M_PI * p1[1]);
}

std::pair<double, double> grad_f2(double *p1){
  std::pair<double, double> res;
  res.first = -f_prob2(p1);
  res.second = res.first;
}

double phi(double *p2, Triangle *T, int i){
  switch (i) {
    case 0: //phi_1 over s2 and s3
    {
      return (T->getPoint(2)->getCoord()[0] - T->getPoint(1)->getCoord()[0]) *
                (p2[1] - T->getPoint(2)->getCoord()[1]) - (T->getPoint(2)->getCoord()[1] - T->getPoint(1)->getCoord()[1]) *
                (p2[0] - T->getPoint(2)->getCoord()[0]) / 2 * T->getSurface();
    }
    case 1: //phi_2 over s1 and s3
    {
      return (T->getPoint(2)->getCoord()[0] - p2[0]) * (T->getPoint(0)->getCoord()[1] - T->getPoint(2)->getCoord()[1]) -
                (T->getPoint(2)->getCoord()[1] - p2[1]) * (T->getPoint(0)->getCoord()[0] - T->getPoint(2)->getCoord()[0]) /
                2 * T->getSurface();
    }
    case 2: //phi_3 over s1 and s2
    {
      return (p2[0] - T->getPoint(1)->getCoord()[0]) * (T->getPoint(0)->getCoord()[1] - p2[1])
                - (p2[1] - T->getPoint(1)->getCoord()[1]) * (T->getPoint(0)->getCoord()[0] - p2[0]) /
                2 * T->getSurface();
    }
  }
}

std::pair<double, double> grad_phi(Triangle *T, int i){

  std::pair<double, double> res;
  switch (i) {
    case 0: //phi_1 over s2 and s3
    {
      res.first = (T->getPoint(2)->getCoord()[1] - T->getPoint(1)->getCoord()[1]) / 2 * T->getSurface();
      res.second = (T->getPoint(2)->getCoord()[0] - T->getPoint(1)->getCoord()[0]) / 2 * T->getSurface();
      return res;
    }
    case 1: //phi_2 over s1 and s3
    {
      res.first = (T->getPoint(0)->getCoord()[1] - T->getPoint(2)->getCoord()[1]) / 2 * T->getSurface();
      res.second = (T->getPoint(0)->getCoord()[0] - T->getPoint(2)->getCoord()[0]) / 2 * T->getSurface();
      return res;
    }
    case 2: //phi_3 over s1 and s2
    {
      res.first = (T->getPoint(0)->getCoord()[1] - T->getPoint(1)->getCoord()[1]) / 2 * T->getSurface();
      res.second = (T->getPoint(0)->getCoord()[0] - T->getPoint(1)->getCoord()[0]) / 2 * T->getSurface();
      return res;
    }
  }
}

int main(int argc, char const *argv[]) {

  //double pbError;
  Mesh M;
  double pbError;
  M.__init__("test4.msh");

  // std::cout <<M.getPointM(23)->getIdFreedom() << '\n';
  // std::cout << M.getPointM(23)->getCoord()[0] << ' '<< M.getPointM(23)->getCoord()[1]<<' '<<M.getPointM(23)->getCoord()[2]<<'\n';
  // std::cout <<M.getPointM(22)->getIdFreedom() << '\n';
  // std::cout <<M.getPointM(24)->getIdFreedom() << '\n';

   // int test = l2g2(M.getTriangle(55), 0);
   // std::cout << test << '\n';
  //
   // int test2 = M.getTriangle(55)->getPoint(0)->getLabel();
   // std::cout << test2 << '\n';
  //
  //
  // for (size_t k = 0; k < 100; k++) {
  //   std::cout << M.getListPoint()[k]->getIdFreedom() <<'\n';
  // }

  M.assemble(f_prob, phi, grad_f, grad_phi);
  M.solve();
  pbError = M.Error(exactSol, grad_phi);
  M.totexport();

  std::cout << "L'erreur est : " << pbError << '\n';

  return 0;
}
