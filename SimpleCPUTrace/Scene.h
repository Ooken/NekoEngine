// #pragma message("NEKO_SCENE_H")
#ifndef NEKO_SCENE_H
#define NEKO_SCENE_H

#include "Object.h"

#include "BVH.h"

//DEBUG
#include <iostream>
using std::cout;
using std::endl;


struct Scene
{
  std::list<Object*> objects;
  BVH tree;
  
  ObjectID loadOBJ(std::string file);
  bool unloadOBJ(ObjectID id);
  
  void update();
  
  Scene();
  
  ~Scene();
};

#endif