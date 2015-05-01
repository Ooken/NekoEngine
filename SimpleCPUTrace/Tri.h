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
  
  Tri(Vect t_A=Vect(),Vect t_B=Vect(),Vect t_C=Vect()):A(t_A),B(t_B),C(t_C){update();}
  //void update(Vect t_A=A,Vect t_B=B,Vect t_C=C){A=t_A;B=t_B;C=t_C;normal = (A-B).cross(A-C);normal = normal * normal.len();}
  void update(){normal = (A-B).cross(A-C);normal = normal / normal.len();b = B-A;c = C-A;}
  
  dot Intersect(Ray &r, dot *u, dot *v)
  {
    if(normal.X==0&&normal.Y==0&&normal.Z==0)
      return -1;//kein gutes Dreieck ^^"
      Vect w = (r.Pos+r.Dir)-A;
    //precalc the distance
    dot diver = normal * r.Dir;
    //plane and ray parallel or stuff
    if(diver == 0)
      return -1;
    dot dist = (normal*(A-r.Pos))/diver;
    if(dist <= 0)
      return -1;
    
    
    //some precalculations (dot producs...)
    dot bb = b*b;
    dot cc = c*c;
    dot bc = b*c;
    dot wb = w*b;
    dot wc = w*c;
    
    //reuse the var for the UV calculation
    diver = (bc*bc)-(bb*cc);
    *u = ((bc*wc)-(cc*wb))/diver;
    if(*u < 0 || *u > 1)
      return -1;//is far way to far away
      *v = ((bc*wb)-(bb*wc))/diver;
    if(*v < 0 || (*v + *u) > 1)
      return -1;//and again... to far away...
      //done
      return dist;
  }
  
};

#endif