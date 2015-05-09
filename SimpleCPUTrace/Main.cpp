#include "Scene.h"

#include <iostream>
#include <fstream>
#include <time.h>
#include <bitset>
#include <stdio.h>      /* printf, scanf, puts */
#include <stdlib.h>     /* realloc, free, exit, NULL */
using namespace std;


void spc(int num)
{
  //decrease AFTER testing...
  while(num-- > 0)
    std::cout << "|";
}

void ShowBox(AAB &box)
{
  std::cout << "[ " << box.lower.X << " | " << box.lower.Y << " | " << box.lower.Z << " ]-[ " << box.higher.Y << " | " << box.higher.Y << " | " << box.higher.Z << " ]" << std::endl;
}

void ViewTree(NODE *node, int depth=0)
{
  spc(depth);
  //   if(depth >= 7){std::cout << "DEEP NODE" << std::endl; return;}
  if(node==NULL){std::cout << "CORRUPT NODE" << std::endl; return;}
  std::cout << (node->O==NULL?"NODE: ":"OBJ: ");
  if(node->O != NULL)
  {
    std::cout << "- ID: " << (bitset<32>)node->O->morton;
    ShowBox(node->bounds);
  }else{
    ShowBox(node->bounds);
    ViewTree(node->A,depth+1);
    ViewTree(node->B,depth+1);
    spc(depth);std::cout << "\u02EA-" << std::endl;
  }
}

int main(int argc, char **argv)
{
  cout << "SIMPLETRACE" << endl;
  
  Scene scn;
  
  scn.active_cam.setPos(Vect(10,0.1,0.2));
  scn.active_cam.lookAt(Vect(-1,0.2,0.1));
  
  ObjectID id = scn.loadOBJ("sample.obj.neko");
  cout << "FIRST" << endl;
  scn.update();
  for(auto i = scn.tree.tris.tris.begin(); i != scn.tree.tris.tris.end();++i)
  {
    cout << "TRI:" << scn.tree.tris.tris.size() << endl;
    ShowVectb((*i)->A);
    ShowVectb((*i)->B);
    ShowVectb((*i)->C);
  }
  getchar();
  ViewTree(scn.tree.Internals);
  
  scn.SetSize(256,128);
  scn.render();
  scn.Save("result.ppm");
  scn.SetSize(128,256);
  scn.render();
  scn.Save("resultb.ppm");
  scn.SetSize(256,256);
  scn.render();
  scn.Save("resultc.ppm");
  scn.SetSize(1920/2,1080);
  scn.render();
  scn.Save("resultd.ppm");
  scn.SetSize(1920,1080);
  scn.render();
  scn.Save("resulte.ppm");
  
  
  cout << "REACHED END" << endl;
  return 0;
}