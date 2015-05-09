#include "Cam.h"

#include <iostream>
void ShowVect(Vect &v)
{
  std::cout << "[ " << v.X << " | " << v.Y << " | " << v.Z << " ]" << std::endl;
}

void Cam::setRes(int w, int h, dot d)
{
  if(w > 0)
    resolution.X = w;
  if(h>0)
    resolution.Y = h;
  if(d > 0)
    sDist = d;
  aspec = dot(w)/dot(h);
  std::cout << "aspec:" << aspec << std::endl;
  //   std::getchar();
}

void Cam::setPos(Vect t_pos)
{
  pos = t_pos;
}


void Cam::lookAt(Vect t_at)
{
  dir = (t_at - pos).Normalize();
  
  
  right = dir.cross(Vect(0,0,1)).Normalize();
  up = -right.cross(dir).Normalize();
  std::cout << "I am at         ";ShowVect(pos);
  std::cout << "looking towards ";ShowVect(dir);
  std::cout << "right           ";ShowVect(right);
  std::cout << "up              ";ShowVect(up);
}
Ray Cam::getRay(dot x, dot y)
{
  dot width = resolution.X-1;
  dot height = resolution.Y-1;
  dot xamnt = 0;
  dot yamnt = 0;
  if (width > height) {
    // the image is wider than it is tall
    xamnt = ((x+0.5)/width)*aspec - (((width-height)/(double)height)/2);
    yamnt = ((height - y) + 0.5)/height;
  }
  else if (height > width) {
    // the imager is taller than it is wide
    xamnt = (x + 0.5)/ width;
    yamnt = (((height - y) + 0.5)/height)/aspec - (((height - width)/(double)width)/2);
  }
  else {
    // the image is square
    xamnt = (x + 0.5)/width;
    yamnt = ((height - y) + 0.5)/height;
  }
//   camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();
  return Ray(pos,(right*(xamnt - 0.5)+(-(up*(yamnt - 0.5)))+dir).Normalize());
}