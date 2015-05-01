#include "BVH.h"

#include <stdlib.h>     /* realloc, free, exit, NULL */

void BVH::manageMem()
{
  if(malcount == -1)
  {
    malcount = tris.tris.size();
    Leafs = (NODE*) malloc(sizeof(NODE)*malcount);
    Internals = (NODE*) malloc(sizeof(NODE)*(malcount-1));
    return;
  }
  
}