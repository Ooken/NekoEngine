// #pragma message("NEKO_OBJECT_H")
#ifndef NEKO_OBJECT_H
#define NEKO_OBJECT_H

#include "BVH.h"
#include <list>
#include <string>

typedef unsigned long int ObjectID;

struct Object
{
  std::list<Tri> tris;
  TriList* visualizerLST;
  bool valid = true;
  Object(TriList* p=NULL,std::string file = "");
  ~Object(){Clear();}
  void Clear();
  bool load(std::string file);
  
  ObjectID id;
  static ObjectID currid;
};

#endif