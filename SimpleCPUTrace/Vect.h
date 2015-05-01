// #pragma message("NEKO_VECT_H")
#ifndef NEKO_VECT_H
#define NEKO_VECT_H
#include "math.h"
#include "decl.h"


//for better performance...
struct Vect
{
  dot X;
  dot Y;
  dot Z;
  
  Vect(dot x=0,dot y = 0,dot z = 0):X(x),Y(y),Z(z){}
  
  Vect operator +(Vect const &v){return Vect(X+v.X,Y+v.Y,Z+v.Z);}
  Vect operator -(Vect const &v){return Vect(X-v.X,Y-v.Y,Z-v.Z);}
  Vect operator /(dot const &v){return Vect(X/v,Y/v,Z/v);}
  Vect operator *(dot const &v){return Vect(X*v,Y*v,Z*v);}
  //Dot Product
  dot operator *(Vect const &v){return X*v.X+Y*v.Y+Z*v.Z;}
  //Cross Product
  Vect cross(Vect const &v)
  {
    return Vect(Y*v.Z-Z*v.Y,
		Z*v.X-X*v.Z,
		X*v.Y-Y*v.X);
  }
  dot len(){return sqrt((*this)*(*this));}
  Vect Normalize(){return (*this)/len();}
  //divide 1. / VECT
  Vect OneDiv(){return Vect(1./X,1./Y,1./Z);}
};

#endif