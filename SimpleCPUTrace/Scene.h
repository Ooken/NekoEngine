// #pragma message("NEKO_SCENE_H")
#ifndef NEKO_SCENE_H
#define NEKO_SCENE_H

#include "Object.h"

#include "BVH.h"

#include "Cam.h"

#include "image.h"

//DEBUG
#include <iostream>
using std::cout;
using std::endl;


struct Scene
{
  std::list<Object*> objects;
  BVH tree;
  
  Cam active_cam;
  
  Image result;
  
  ObjectID loadOBJ(std::string file);
  bool unloadOBJ(ObjectID id);
  
  void update();
  
  void SetSize(unsigned int w, unsigned int h);
  
  Scene();
  
  ~Scene();
  
};

#endif