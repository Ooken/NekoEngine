// #pragma message("NEKO_AAB_H")
#ifndef NEKO_AAB_H
#define NEKO_AAB_H

#include "Vect.h"
#include "Ray.h"

//AxisAlignedBox, used for VBH
struct AAB
{
  Vect lower;
  Vect higher;
  
  bool set;
  AAB(){set=false;}
  void Increase(AAB const &in);
  void Increase(Vect const &in);
  
  bool Intersect(Ray const &r);
};

#endif