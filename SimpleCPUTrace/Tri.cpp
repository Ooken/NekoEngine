#include "Tri.h"

dot Tri::Intersect(Ray &r, dot *u, dot *v)
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