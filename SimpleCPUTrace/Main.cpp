#include "Scene.h"

#include <iostream>
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

void ViewTree(NODE *node, int depth=0)
{
  spc(depth);
  //   if(depth >= 7){std::cout << "DEEP NODE" << std::endl; return;}
  if(node==NULL){std::cout << "CORRUPT NODE" << std::endl; return;}
  std::cout << (node->O==NULL?"NODE: \n":"OBJ: ");
  if(node->O != NULL)
  {
    std::cout << "- ID: " << (bitset<32>)node->O->morton << std::endl;
  }else{
    ViewTree(node->A,depth+1);
    ViewTree(node->B,depth+1);
    spc(depth);std::cout << "\u02EA-" << std::endl;
  }
}


int main(int argc, char **argv)
{
  cout << "SIMPLETRACE" << endl;
  
  
  
  Scene scn;
  
  ObjectID id = scn.loadOBJ("sample.obj.neko");
  cout << "FIRST" << endl;
  scn.update();
  
  ViewTree(scn.tree.Internals);
  
  scn.loadOBJ("sampleb.obj.neko");
  
  cout << "SECOND" << endl;
  scn.update();
  ViewTree(scn.tree.Internals);
  
  scn.unloadOBJ(id);
  
  cout << "THIRD" << endl;
  scn.update();
  ViewTree(scn.tree.Internals);
  //   std::getchar();
  cout << "REACHED END" << endl;
  return 0;
}