#include "BVH.h"

#include <stdlib.h>     /* realloc, free, exit, NULL */

void BVH::manageMem()
{
  if(malcount == -1)
  {
    n_alloc();
  }
  if(malcount != tris.tris.size())
  {
    n_realloc();
  }
}

void BVH::n_alloc()
{
  malcount = tris.tris.size();
  //   std::cout << "alloc:" << malcount << std::endl;
  Leafs = (NODE*) malloc(sizeof(NODE)*malcount);
  Internals = (NODE*) malloc(sizeof(NODE)*(malcount-1));
  Leafs[malcount-1].reset();
  for(int i = malcount - 2; i >= 0;--i)
  {
    Leafs[i].reset();
    Internals[i].reset();
  }
}

void BVH::n_free()
{
  //   std::cout << "free:" << malcount << std::endl;
  if(Leafs != NULL)
    free(Leafs);
  Leafs = NULL;
  if(Internals != NULL)
    free(Internals);
  Internals = NULL;
  malcount = -1;
}

void BVH::n_realloc()
{
  malcount = tris.tris.size();
  //   std::cout << "realloc:" << malcount << std::endl;
  Leafs = (NODE*) realloc(Leafs, sizeof(NODE)*malcount);
  Internals = (NODE*) realloc(Internals, sizeof(NODE)*(malcount-1));
  if(Leafs==NULL || Internals == NULL)
  {
    //do it by hand if it doesn't like the realloc way .-.
    n_free();
    n_alloc();
  }
  Leafs[malcount-1].reset();
  for(int i = malcount - 2; i >= 0;--i)
  {
    Leafs[i].reset();
    Internals[i].reset();
  }
}

void BVH::prepare()
{
  if(malcount != tris.tris.size())
    manageMem();
  tris.tris.sort([](Tri const *i, Tri const *j){return (i->morton<j->morton);});
  //   std::cout << "copy:" << malcount << std::endl;
  std::list<Tri*>::iterator it = tris.tris.begin();
  for(int i = 0; i < malcount; ++i,++it)
    Leafs[i].O = (*it);
}

void BVH::generate()
{
  prepare();
  
  for(int i = 0; i < malcount-1; ++i)
    generateNode(i);
  
  //TODO: Generate AABB's
  
}

void BVH::generateNode(int idx)
{
  //from where to where will this node go?
  int2 range = determineRange(idx);
  //where will be the split within the range?
  int split = findSplit(range.X, range.Y);
  
  //The lower Nodes/Leafs will be at split position and at split+1 position
  NODE* ChA;
  NODE* ChB;
  //if the left Child is at the first position of the range, i.e. the
  //split is at the beg of the range, it will be a leaf node to object[X]
  if(split == range.X)
    ChA = &Leafs[split];
  else
    ChA = &Internals[split];
  
  //The same with the right child, if it hits the last index in the range
  //it will be a child, so if the range is only 2 wide [for example from 3 to 4]
  //both childs will be leafs, and if it goes over 3, then one will be a child
  if(split+1 == range.Y)
    ChB = &Leafs[split+1];
  else
    ChB = &Internals[split+1];
  
  //Also set the Parent node
  ChA->P = &Internals[idx];
  ChB->P = &Internals[idx];
  Internals[idx].A = ChA;
  Internals[idx].B = ChB;
  
}

int BVH::findSplit(int first, int last)
{
  
  unsigned int firstCode = Leafs[first].O->morton;
  unsigned int lastCode = Leafs[last].O->morton;
  
  //When morton codes are identical we want to return the first id
  //because if we split it in the middle, both will try to orientate
  //towards the same direction
  
  //to split it in the middle would be the best but since it would be harder
  //for the range detector we do it this way (both ways less instructions needed:
  //here: (first + last) >> 1 to "first" and
  //in determine range we can reduce it to seek forward instead of tetermining the
  //sub direction and stuff...
  if (firstCode == lastCode)
    //return (first + last) >> 1;
    return first;
  
  // Calculate the number of highest bits that are the same
  // for all objects, using the count-leading-zeros intrinsic.
  int commonPrefix = CLZ1(firstCode ^ lastCode);
  
  // Use binary search to find where the next bit differs.
  // Specifically, we are looking for the highest object that
  // shares more than commonPrefix bits with the first one.
  
  int split = first; // initial guess
  int step = last - first;
  do
  {
    step = (step + 1) >> 1; // exponential decrease
    int newSplit = split + step; // proposed new position
    
    if (newSplit < last)
    {
      unsigned int splitCode = Leafs[newSplit].O->morton;
      int splitPrefix = CLZ1(firstCode ^ splitCode);
      if (splitPrefix > commonPrefix)
	split = newSplit; // accept proposal
    }
  }
  while (step > 1);
  
  return split;
}

int2 BVH::determineRange(int index)
{
  //so we don't have to call it every time
  int lso = malcount-1;
  //tadaah, it's the root node
  if(index == 0)
    return int2( 0, lso);
  //direction to walk to, 1 to the right, -1 to the left
  int dir;
  //morton code diff on the outer known side of our range ... diff mc3 diff mc4 ->DIFF<- [mc5 diff mc6 diff ... ] diff .. 
  int d_min;
  int initialindex = index;
  
  unsigned int minone = Leafs[index-1].O->morton;
  unsigned int precis = Leafs[index].O->morton;
  unsigned int pluone = Leafs[index+1].O->morton;
  if((minone == precis && pluone == precis))
  {
    //set the mode to go towards the right, when the left and the right
    //object are being the same as this one, so groups of equal
    //code will be processed from the left to the right
    //and in node order from the top to the bottom, with each node X (ret.x = index)
    //containing Leaf object X and nodes from X+1 (the split func will make this split there)
    //till the end of the groups
    //(if any bit differs... DEP=32) it will stop the search
    while(index > 0 && index < lso)
    {
      //move one step into our direction
      index += 1;
      if(index >= lso)
	//we hit the left end of our list
	break;
      
      if(Leafs[index].O->morton != Leafs[index+1].O->morton)
	//there is a diffrence
	break;
    }
    //return the end of equal grouped codes
    return int2(initialindex,index);
  }else{
    //Our codes differ, so we seek for the ranges end in the binary search fashion:
    int2 lr= int2(CLZ1(precis ^ minone),CLZ1(precis ^ pluone));
    //now check wich one is higher (codes put side by side and wrote from up to down)
    if(lr.X > lr.Y)
    {//to the left, set the search-depth to the right depth
      dir = -1;
      d_min = lr.Y;
    }else{//to the right, set the search-depth to the left depth
      dir = 1;
      d_min = lr.X;
    }
  }
  //Now look for an range to search in (power of two)
  int l_max = 2;
  //so we don't have to calc it 3x
  int testindex = index + l_max*dir;
  while((testindex<=lso&&testindex>=0)?(CLZ1(precis ^ Leafs[testindex].O->morton)>d_min):(false))
  {l_max *= 2;testindex = index + l_max*dir;}
  int l = 0;
  //go from l_max/2 ... l_max/4 ... l_max/8 .......... 1 all the way down
  for(int div = 2 ; l_max / div >= 1 ; div *= 2){
    //calculate the ofset state
    int t = l_max/div;
    //calculate where to test next
    int newTest = index + (l + t)*dir;
    //test if in code range
    if (newTest <= lso && newTest >= 0)
    {
      int splitPrefix = CLZ1(precis ^ Leafs[newTest].O->morton);
      //and if the code is higher then our minimum, update the position
      if (splitPrefix > d_min)
	l = l+t;
    }
  }
  //now give back the range (in the right order, [lower|higher])
  if(dir==1)
    return int2(index,index + l*dir);
  else
    return int2(index + l*dir,index);
}