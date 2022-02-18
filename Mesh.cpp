#include "Mesh.hh"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <numeric>

#define dirichlet_hot 30
#define dirichlet_cold 5

using namespace std;

Mesh::Mesh() : SpA(NULL), F(NULL), X(NULL){}

//////////////////////////////////////////////////////////////

void Mesh::__init__(string pathdir){

  ifstream myFile(pathdir, ios::in);

  string line;
  string seek = "$Nodes";
  int cmpt = 1;
  int dol = 1;
  double x, y, z;
  int nbTriangle;
  int buf1, buf2, buf3, buf4;
  double nbBlock, nbPoint;
  int bord, dirc;

  if(!myFile.fail())
  {

    // while (getline(myFile, line)) {
    //   if (line == "$Nodes") {
    //     getline(myFile, line);
    //     break;
    //   }
    // }

    while(getline(myFile,line))
    {
      int i =0;
      while(i<6)
      {
        if (line[i] != seek[i])
          {
          break;}
        i++;
        }
      if (i==6)
        {break;}
    }

    myFile >> nbBlock >> nbPoint;
    getline(myFile, line);
    for (size_t k = 0; k < nbBlock; k++) {
      myFile >> buf1 >> buf2 >> buf3 >> buf4;
      bord = (buf1 == 2)? 1:0;
      dirc = buf2;
      for (size_t i = 0; i < buf4 + 1; i++) {
        getline(myFile, line);
      }
      for (size_t j = 0; j < buf4; j++) {
        myFile >> x >> y >> z;
        //Create and add a point to the mesh
        if (!bord) { //if the point is on an edge
          if(dirc > 30 && dirc < 35){ //if dirichlet cold
            listPoint.push_back(new Point(x,y,z,-2,cmpt));
            cmpt ++;
            getline(myFile, line);
          }
          if (dirc < 31 && dirc >22) { //if dirichlet hot
            listPoint.push_back(new Point(x,y,z,-3,cmpt));
            cmpt ++;
            getline(myFile, line);
          }
          else{ //only neumann condition
            listPoint.push_back(new Point(x,y,z,-1,cmpt));
            cmpt ++;
            getline(myFile, line);
          }
        }
        else{
          listPoint.push_back(new Point(x,y,z,dol,cmpt));
          cmpt ++;
          dol ++;
          getline(myFile, line);
        }
      }
    }

    getline(myFile, line);
    getline(myFile, line);

    myFile >> buf1 >> buf2 >> buf3 >> buf4;
    int n = buf1;
    int max;

    getline(myFile, line);
    for (size_t i = 0; i < n; i++) {
       myFile >> buf1 >> buf2 >> buf3 >> buf4;

       if (buf3 == 2) {
         max = buf4;
         for (size_t j = 0; j < max; j++) {
           myFile >> buf1 >> buf2 >> buf3 >> buf4;
           std::cout << buf1 <<' '<<buf2<<' '<<buf3 <<' '<<buf4<<'\n';
           //create and add a triangle to the mesh
           Triangle *T = new Triangle();
           T->__init__(this->getPointM(buf2-1), this->getPointM(buf3-1), this->getPointM(buf4-1),buf1);
           listTriangle.push_back(T);
           getline(myFile, line);
         }
       }
       else{
         for (size_t j = 0; j < buf4 + 1; j++) {
           getline(myFile, line);
         }
       }
     }
    myFile.close();
  }

  else{
    cerr << "Erreur dans la lecture du fichier" << '\n' << endl;
  }
}

////////////////////////////////////////////////////////////////

void Mesh::solve(){

  int max = 0;

  for (size_t k = 0; k < listPoint.size(); k++) {
    if (listPoint[k]->getIdFreedom() > max) {
      max = listPoint[k]->getIdFreedom();
    }
  }
  X = new Eigen::VectorXd (max);
  Eigen::ConjugateGradient<Eigen::SparseMatrix <double> > solver;
  solver.compute(*SpA);
  if(solver.info()!=Eigen::Success){
    //decomposition failed
    return;
  }
  *X = solver.solve(*F);
  if(solver.info()!=Eigen::Success){
    //solving failed
    return;
  }
}

/////////////////////////////////////////////////////////////////////

int l2g(Triangle *T, int index){
  int fd = T->getPoint(index)->getIdFreedom();
  return fd;
}

//////////////////////////////////////////////////////////////////

void Mesh::assemble(double (&f_prob)(double *p1), double (&phi)(double *p2, Triangle *T, int i),
  pair<double, double> (&grad_f)(double *p1), pair<double, double> (&grad_phi)(Triangle *T, int i)){

  int I, J;
  double sum = 0;
  vector<Triangle*>::iterator iter = listTriangle.begin();
  vector<Eigen::Triplet <double> > contrib;
  pair<double, double> buf_phi, buf_f;
  double scal_prod, scal_prod2;
  int max = 0;

  for (size_t k = 0; k < listPoint.size(); k++) {
    if (listPoint[k]->getIdFreedom() > max) {
      max = listPoint[k]->getIdFreedom();
    }
  }

  SpA = new Eigen::SparseMatrix<double> (max + 1, max + 1);
  F = new Eigen::VectorXd(max + 1);


  for (; iter != listTriangle.end(); iter++) {

    (*iter)->setEdge();
    (*iter)->setSurface();
    for (size_t k = 0; k < 3; k++) {

      I = l2g(*iter, k);
      if(I==-1) continue;
      for (size_t j = 0; j < 3; j++) {

        sum += f_prob(this->getBarycentre((*iter)->getListEdge()[j])) *
          phi(this->getBarycentre((*iter)->getListEdge()[j]), *iter, k);
      }

      (*F)(I) = (*iter)->getSurface() /3 * sum;
      for (size_t l = 0; l < 3; l++) {

        J = l2g(*iter, l);
        if(J < 0){
          scal_prod2 = grad_f(this->getBarycentre((*iter)->getListEdge()[l])).first *
            grad_phi(*iter, l).first + grad_f(this->getBarycentre((*iter)->getListEdge()[l])).second *
              grad_phi(*iter, l).second;
          if ((*iter)->getPoint(l)->getIdFreedom() == -3) {
            (*F)(J) -= dirichlet_hot * (*iter)->getSurface() * scal_prod2;
          }
          if((*iter)->getPoint(l)->getIdFreedom() == -2){
            (*F)(J) -= dirichlet_cold * (*iter)->getSurface() * scal_prod2;
          }
          continue;
        }
        buf_phi = grad_phi(*iter, l);
        buf_f = grad_f(this->getBarycentre((*iter)->getListEdge()[l]));
        scal_prod = buf_phi.first * buf_f.first + buf_phi.second * buf_f.second;
        contrib.push_back(Eigen::Triplet<double>((double)I, (double)J, (*iter)->getSurface()
          * scal_prod));
      }
    }
  }
  SpA->setFromTriplets(contrib.begin(), contrib.end());
}

////////////////////////////////////////////////////

double Mesh::Error(double (&exactSol)(double x, double y),
      pair<double, double> (&grad_phi)(Triangle *T, int i)){

  vector<double> delta_u = {0., 0., 0.};
  pair <double, double> error2 = {0., 0.};
  int I;
  vector<Triangle*>::iterator iter = listTriangle.begin();
  pair <double, double> tmp = {0.,0.};
  double res = 0.;

  for (; iter != listTriangle.end(); iter++) {

    for (size_t i = 0; i < 3; i++) {

      I = l2g(*iter, i);
      if(I==-1) continue;
        delta_u[i] = exactSol((*iter)->getPoint(i)->getCoord()[0],
          (*iter)->getPoint(i)->getCoord()[1]) - (*X)(I);
          std::cout << (*X)(I) << '\n';

    }
    for (size_t j = 0; j < 3; j++) {
      error2.first += delta_u[j] * grad_phi(*iter, j).first;
      error2.second += delta_u[j] * grad_phi(*iter, j).second;
    }
    res += (*iter)->getSurface() *( error2.first * error2.first + error2.second * error2.second);
  }
  return sqrt(res);
}

//////////////////////////////////////////////////

Mesh::~Mesh(){
  for (size_t i = 0; i < listTriangle.size(); i++) {
    delete listTriangle[i];
  }
  for (size_t i = 0; i < listPoint.size(); i++) {
    delete listPoint[i];
  }
  if(SpA){
    delete SpA;
  }
  if(F){
    delete F;
  }
  if (X) {
    delete X;
  }
}
