// #pragma message("NEKO_TRI_H")
#ifndef NEKO_TRI_H
#define NEKO_TRI_H

#include "Ray.h"

struct Tri
{
  Vect A,B,C;
  
  Vect b;
  Vect c;
  Vect normal;
  
  unsigned int moroton;
  
  Tri(Vect t_A=Vect(),Vect t_B=Vect(),Vect t_C=Vect()):A(t_A),B(t_B),C(t_C){update();}
  //void update(Vect t_A=A,Vect t_B=B,Vect t_C=C){A=t_A;B=t_B;C=t_C;normal = (A-B).cross(A-C);normal = normal * normal.len();}
  void update(){normal = (A-B).cross(A-C);normal = normal / normal.len();b = B-A;c = C-A;}
  
  dot Intersect(Ray &r, dot *u, dot *v);
  
};

#endif