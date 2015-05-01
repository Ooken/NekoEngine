// #pragma message("NEKO_RENDER_CORE")
#ifndef NEKO_RENDER_CORE
#define NEKO_RENDER_CORE

#include "Tri.h"
#include "AAB.h"
#include <list>



//Core management for triangles (pointing to the objects)
struct TriList
{
  std::list<Tri*> tris;
  void Register(Tri *p){tris.push_back(p);}
  void UnRegister(Tri *p){tris.remove(p);}
};

struct NODE
{
  NODE *A;
  NODE *B;
  Tri *O;
  AAB bounds;
};

struct BVH
{
  TriList tris;
  
  int malcount;
  
  NODE *Internals;
  NODE *Leafs;
  
  BVH(){malcount = -1; Internals = NULL; Leafs = NULL;}
  
  void manageMem();
  
  void n_allocate();
  void n_free();
  void n_realloc();
  
  void prepare();
  void generate();
  
  
};

#endif