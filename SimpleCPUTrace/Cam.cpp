#include "Cam.h"

void Cam::setRes(int w, int h)
{
  if(w > 0)
    resolution.X = w;
  if(h>0)
    resolution.Y = h;
    aspec = resolution.X / resolution.Y;
}

void Cam::setPos(Vect t_pos)
{
  pos = t_pos;
}

void Cam::lookAt(Vect t_at)
{
  dir = (t_at - pos).Normalize();
  right = Vect(dir.X, dir.Y, dir.Z - 1).cross(dir).Normalize();
  down = right.cross(dir).Normalize();
}

Ray Cam::getRay(int x, int y)
{
  dot width = resolution.X;
  dot height = resolution.Y;
  dot xamnt;
  dot yamnt;
  if (width > height)
  {//wider...
    xamnt = ((x + 0.5) / width)*(aspec) - ((dot(width - height) / dot(height)) / 2.);
    yamnt = ((height - y) + 0.5) / height;
  }
  else if (height > width){//higher...
    xamnt = (x + 0.5) / width;
    yamnt = (((height - y) + 0.5) / height) / (aspec) - ((dot(height - width) / dot(width)) / 2.);
  }
  else{//1:1...
    xamnt = (x + 0.5) / width;
    yamnt = ((height - y) + 0.5) / height;
  }
  
  //apply:
  return Ray(pos,(dir + ((right*(xamnt - 0.5)) + (down*(yamnt - 0.5)))).Normalize());
}