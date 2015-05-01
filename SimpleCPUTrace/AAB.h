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
  void Increase(AAB const &in){
    if(!set){lower = in.lower; higher=in.higher;set=true;return;}
    if(in.lower.X < lower.X)lower.X = in.lower.X;
    if(in.higher.X > higher.X)higher.X = in.higher.X;
    if(in.lower.Y < lower.Y)lower.Y = in.lower.Y;
    if(in.higher.Y > higher.Y)higher.Y = in.higher.Y;
    if(in.lower.Z < lower.Z)lower.Z = in.lower.Z;
    if(in.higher.Z > higher.Z)higher.Z = in.higher.Z;
  }
  void Increase(Vect const &in){
    if(!set){lower = in; higher = in;set = true; return;}
    if(in.X < lower.X)lower.X = in.X;
    else if(in.X > higher.X)higher.X = in.X;
    if(in.Y < lower.Y)lower.Y = in.Y;
    else if(in.Y > higher.Y)higher.Y = in.Y;
    if(in.Z < lower.Z)lower.Z = in.Z;
    else if(in.Z > higher.Z)higher.Z = in.Z;
  }
  AAB(){set=false;}
  
  bool Intersect(Ray const &r)
  {
    dot t1 = (lower.X - r.Pos.X)*r.invDir.X;
    dot t2 = (higher.X - r.Pos.X)*r.invDir.X;
    dot t3 = (lower.Y - r.Pos.Y)*r.invDir.Y;
    dot t4 = (higher.Y - r.Pos.Y)*r.invDir.Y;
    dot t5 = (lower.Z - r.Pos.Z)*r.invDir.Z;
    dot t6 = (higher.Z - r.Pos.Z)*r.invDir.Z;
    
    dot tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
    dot tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
    
    if(tmax < 0)
      return false; //box is behind us
    
    if(tmin > tmax)
      return false; //doesnt intersect
    
    return true;//tadaah, got hit
  }
};

#endif