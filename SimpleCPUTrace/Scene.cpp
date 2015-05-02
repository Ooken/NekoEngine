#include "Scene.h"

ObjectID Scene::loadOBJ(std::string file)
{
  if(file.find(".obj.neko")!=std::string::npos)
  {
    objects.push_back(new Object(&tree.tris,file));
    if(objects.back()->valid)
      return objects.back()->id;
    std::cout << "Object load failed..." << file << std::endl;
    objects.pop_back();
    return false;
  }else{
    std::cout << "Check Spelling[name.obj.neko]..." << file << std::endl;
  }
}

Scene::Scene()
{
  
}

struct find_Object
{
    ObjectID id;
    find_Object(ObjectID t_id) : id(t_id) {}
    bool operator () ( const Object* m ) const
    {
        return m->id == id;
    }
};

bool Scene::unloadOBJ(ObjectID id)
{
  std::list<Object*>::iterator found = std::find_if(objects.begin(),objects.end(),find_Object(id));
  
  if(found == objects.end())
    return false;
  delete *found;
  objects.erase(found);
  return true;
}

void Scene::update()
{
  tree.prepare();
  tree.generate();
}

Scene::~Scene()
{
  while(!objects.empty())
  {
    delete objects.back();
    objects.pop_back();
  }
}