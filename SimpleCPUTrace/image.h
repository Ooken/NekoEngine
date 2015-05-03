#ifndef NEKO_IMAGE_H
#define NEKO_IMAGE_H

//for storing and saving images

#include "decl.h"


//almost Vect but slightly diffrent
struct Color
{
  dot R;
  dot G;
  dot B;
  void reset(){R = 0; G = 0; B = 0;}
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
};

#endif