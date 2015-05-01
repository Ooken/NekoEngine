// #pragma message("NEKO_RENDER_CORE")
#ifndef NEKO_RENDER_CORE
#define NEKO_RENDER_CORE

#include "Tri.h"
#include <list>


//Core management for triangles (pointing to the objects)
struct TriList
{
  std::list<Tri*> tris;
  void Register(Tri *p){tris.push_back(p);}
  void UnRegister(Tri *p){tris.remove(p);}
};

#endif