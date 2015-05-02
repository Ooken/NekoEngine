// #pragma message("NEKO_RENDER_CORE")
#ifndef NEKO_RENDER_CORE
#define NEKO_RENDER_CORE

#include "Tri.h"
#include "AAB.h"
#include <list>

#include <iostream>
using namespace std;


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
  NODE* P;
  Tri *O;
  AAB bounds;
  void reset(){A=NULL;B=NULL;P=NULL;O=NULL;}
};

struct BVH
{
  TriList tris;
  
  int malcount;
  
  NODE *Internals;
  NODE *Leafs;
  
  BVH(){malcount = -1; Internals = NULL; Leafs = NULL;}
  ~BVH(){n_free();}
  void manageMem();
  
  void n_alloc();
  void n_free();
  void n_realloc();
  
  void prepare();
  void generate();
  
  //KERNEL-TREE
  void generateNode(int idx);
  int findSplit(int first, int last);
  int2 determineRange(int index);
  
  //KERNEL-BOUNDINGBOXES
  
};

#endif