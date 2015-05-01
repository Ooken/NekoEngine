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
  
  bool load(std::string file)
  {
    if(file.find(".scn.neko")!=std::string::npos)
      cout << "is scene file" << endl;
    if(file.find(".obj.neko")!=std::string::npos)
    {
      objects.push_back(new Object(&tree.tris,file));
    }
  }
  
  void update()
  {
    
  }
  
  ~Scene()
  {
    while(!objects.empty())
    {
      delete objects.back();
      objects.pop_back();
    }
  }
};

#endif