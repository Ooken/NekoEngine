#ifndef NEKO_RENDER_H
#define NEKO_RENDER_H

#include "image.h"

#include "BVH.h"

#include "Cam.h"


struct RenderInstance
{
  
  RenderInstance();
  
  Color getPixel(int x, int y);
  
  void SetCam(Cam *c = NULL){active = c;}
  void SetOBJ(BVH *b = NULL){objects = b;}
  
private:
  //internal Func to exit when something's wrong ^^"
  bool checkstatus();
  
  Cam *active;
  BVH *objects;
};

#endif