#include "Render.h"


RenderInstance::RenderInstance()
{
  active = NULL;
  objects = NULL;
}

bool RenderInstance::checkstatus()
{
  if(active==NULL || objects == NULL)
    return false;
  
  //all tests passed, everything's fine
  return true;
}

Color RenderInstance::getPixel(int x, int y)
{
  if(!checkstatus())
    return Color(0,0,0);
  Color ret(1,0,0);
  
  //Rendering here:
  
  Ray r = active->getRay(x,y);
  
  ret.R = abs(r.Dir.X);
  ret.G = abs(r.Dir.Y);
  ret.B = abs(r.Dir.Z);
  
//   return ret;
  
  /*ShowVectb(r.Pos);
  r.Dir = active->dir;ShowVectb(r.Pos + r.Dir * 4);*/
//   cout << "TRI:" << endl;
//   ShowVectb(objects->tris.tris.front()->A);
//   ShowVectb(objects->tris.tris.front()->B);
//   ShowVectb(objects->tris.tris.front()->C);
  dot nu,nv;
//   if(-1 != objects->tris.tris.front()->Intersect(r,&nu,&nv))
//     cout << "GOT HIT AT " << objects->tris.tris.front()->Intersect(r,&nu,&nv) << endl;
  //TODO:CHECK IF OK
  //seek for hits
  
  std::list<NODE*> tocheck;
  std::list<Tri*> gotTris;//tris with hit bounding boxes
  tocheck.push_back(objects->Internals);
  Tri *currentNeares = NULL;
  dot currentDist =100;
  
  dot nU=0;
  dot nV=0;
  for(auto i = objects->tris.tris.begin(); i != objects->tris.tris.end(); ++i)
  {
    dot tu;
    dot tv;
    dot temp = (*i)->Intersect(r,&tu,&tv);
    if(temp < currentDist && temp > 0)
    {
      nU = tu;
      nV = tv;
      currentDist = temp;
      ret.R = temp / 5.;
      ret.G = nU;
      ret.B = nV;
    }
  }
  return ret;
  cout << "NO WAY" << endl;
  while(!tocheck.empty())
  {
    
    
    NODE* curr = tocheck.front();
    tocheck.pop_front();
    bool hit = curr->bounds.Intersect(r);
    if(hit)
    {//process children
      if(curr->O!=NULL)
      {//it's a leaf
	gotTris.push_back(curr->O);
      }else{
	tocheck.push_back(curr->A);
	tocheck.push_back(curr->B);
      }
    }
  }
  //now we setup a list of tris...
  
  ret.R = dot(gotTris.size()) / dot(objects->tris.tris.size());
  while(!gotTris.empty())
  {
    Tri *curr = gotTris.front();
    gotTris.pop_front();
    dot u=0;
    dot v=0;
    dot newDist = curr->Intersect(r,&u,&v);
    
    if(newDist < currentDist && newDist > 0)
    {
      currentDist = newDist;
      nU = u;
      nV = v;
      currentNeares = curr;
    }
  }
  if(currentNeares != NULL)
  {
//     cout << "[" << x << "|" << y << "]" << endl;
    ret.G = nU;
    ret.B = nV;
  }
  //renderting done
  return ret;
}