#ifndef NEKO_IMAGE_H
#define NEKO_IMAGE_H

//for storing and saving images

#include "decl.h"

#include <string>


//almost Vect but slightly diffrent
struct Color
{
  dot R;
  dot G;
  dot B;
  dot A;
  void reset(){R = 0; G = 0; B = 0; A=0;}
  Color(dot r=0,dot g=0,dot b=0,dot a=0):R(r),G(g),B(b),A(a){}
};

struct Image
{
  Color* data;
  unsigned int size;
  
  int2 dim;
  
  Image()
  {
    size = -1;
    data = NULL;
  }
  
  void SetDim(unsigned int w, unsigned int h);
  
  
  void n_alloc();
  void n_free();
  void n_realloc();
  
  
  ~Image()
  {
    n_free();
  }
  
  bool Save(std::string file);
  
  void PutPixel(unsigned int x, unsigned int y, Color col);
  
  void genSample();
};

#endif