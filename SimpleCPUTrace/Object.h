// #pragma message("NEKO_OBJECT_H")
#ifndef NEKO_OBJECT_H
#define NEKO_OBJECT_H

#include "BVH.h"
#include <list>
#include <string>


struct Object
{
  std::list<Tri> tris;
  TriList* visualizerLST;
  Object(TriList* p=NULL,std::string file = ""):visualizerLST(p){if(!file.empty())load(file);}
  ~Object(){Clear();}
  void Clear();
  bool load(std::string file);
};

#endif