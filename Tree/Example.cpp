//Example code for generating a binary tree in parallel

//it will calculate the morton code for each object by their x,y and z coordinates
//sort the objects by these
//the tree will be constructed in a way, that it will cut at the highest differing bit
//so if we look at the following:
//01100 A
//01101 B
//01111 C
//10110 D
//10111 E
//it will generate a top node that cut's at C to the ranges of "A-C" and "D-E"
//the sub-ranges will be handled in the same fashion
//so A-C will cut at B, "A-B" will be another child node and "C" will be a leaf node object

//if there are some equal morton codes in the list, the cut finder will return the first object
//and make it child A, and Child B will be the following nodes
//so if we have 5 equal objects the tree will look like this (we will talk with "obj i" about leaf_nodes[i] here)
//Node 0: obj 0 | Node 1
//Node 1: obj 1 | Node 2
//Node 2: obj 2 | Node 3
//Node 3: obj 3 | obj 4
//I guess there is a way to always split them up at half and so on
//(to decrease the calculation needed to go through them)
//but for now I don't know how yet (haven't thought about that much)


//for basic output
#include <iostream>
//for the random init
#include <time.h>
//for the object list:
#include <vector>
//for sorting the list of objects
#include <algorithm>
//for the setprecision output
#include <iomanip>
//for getting the thread count
#include <omp.h>

//some of my standart definitions
#ifndef NULL
#define NULL 0
#endif
typedef float dot;

//constant number for standart object count if not given per argument: 100 mio objects (takes 8,2 GB RAM !!!!!!!!!!)
const unsigned int STD_NUM_OBJ = 100000000;

//forward declaration of structs (node will link to object and vice versa...)
struct int2;
struct Vect;
struct Object;
struct Node;

  //a list of objects
typedef std::vector<Object> t_objects;

//forward declaration of functions
  //from http://devblogs.nvidia.com/parallelforall/thinking-parallel-part-iii-tree-construction-gpu/
unsigned int expandBits(unsigned int v);
  //from http://devblogs.nvidia.com/parallelforall/thinking-parallel-part-iii-tree-construction-gpu/
unsigned int morton3D(dot x, dot y, dot z);


  //claims to be done in within 13 instructions, source at:
  // http://embeddedgurus.com/state-space/2014/09/fast-deterministic-and-portable-counting-leading-zeros/
  // CUDA has __clz but on CPU we have to do it ourself [no forward decl because of inline (tested it... this way it's faster)]
static inline uint32_t CLZ1(uint32_t x) {
    static uint8_t const clz_lkup[] = {
        32U, 31U, 30U, 30U, 29U, 29U, 29U, 29U,
        28U, 28U, 28U, 28U, 28U, 28U, 28U, 28U,
        27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
        27U, 27U, 27U, 27U, 27U, 27U, 27U, 27U,
        26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
        26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
        26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
        26U, 26U, 26U, 26U, 26U, 26U, 26U, 26U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        25U, 25U, 25U, 25U, 25U, 25U, 25U, 25U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U,
        24U, 24U, 24U, 24U, 24U, 24U, 24U, 24U
    };
    uint32_t n;
    if (x >= (1U << 16)) {
        if (x >= (1U << 24)) {
            n = 24U;
        }
        else {
            n = 16U;
        }
    }
    else {
        if (x >= (1U << 8)) {
            n = 8U;
        }
        else {
            n = 0U;
        }
    }
    return (uint32_t)clz_lkup[x >> n] - n;
}



//Find highest differing zero within a range of these objects
  //taken from http://devblogs.nvidia.com/parallelforall/thinking-parallel-part-iii-tree-construction-gpu/
  //and modified to work with our vector list
int findSplit( t_objects &list, int first, int last);
//find out from where to where the node will work, more details down there
  //interpreted from http://devblogs.nvidia.com/parallelforall/wp-content/uploads/sites/3/2012/11/karras2012hpg_paper.pdf
int2 determineRange(t_objects &list, int index);
//to allocate the memory and already write the Leaf nodes
void allocateHierarchy(Node** Internals, Node** Leafs, t_objects &list);
//and to free up the memory
void destructHierarchy(Node* Internals, Node* Leafs);
//and to generate the whole thing
void generateHierarchy( t_objects &list,Node* Tree, Node* Leafs);
//this one could be the kernel:
void generateNode( t_objects &list,Node* Tree, Node* Leafs, int idx);

//CUDA's int2 not defined on CPU
struct int2
{
  int x;
  int y;
  int2(int t_x=0,int t_y=0):x(t_x),y(t_y){}
};

//for simple position storing, nicer to read
struct Vect
{
  dot x;
  dot y;
  dot z;
  Vect(dot t_x=0, dot t_y=0, dot t_z=0):x(t_x),y(t_y),z(t_z){}
};
//our object, pretty simple
struct Object
{
  Vect pos;
  //the calculated morton code will be stored here
  unsigned int mcode;
  //link to the leaf node of this object
  Node* parent;
  Object(Vect t_pos = Vect()){pos = t_pos; mcode = morton3D(pos.x,pos.y,pos.z); parent = NULL;}
};

//Functor for sorting (faster than with function pointer)
struct ObjEval{
  bool operator()(const Object &i,const Object &j) const {
    return (i.mcode<j.mcode);
  }
};

//A Node structure
struct Node
{
  //A -> Left subnode
  Node* A;
  //B -> Right subnode
  Node* B;
  //pointer to object
  //if Null, node is an internal node
  //if not, it is an leaf node
  //it works this way, so there is no memory needed for a boolean var
  //also I dislike virtual classes to work with cuda (they kinda only work halfway so I try not to scratch it to much...)
  Object* O;
  
  //Pointer to the Parent node [useful in bounding box or colision operations
  //to walk the tree from bottom to top]
  Node* parent;
  Node(){ A=NULL; B=NULL; O=NULL; parent = NULL;}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//I'm sorry for doing it in one file but I try to hold this example as easy as possible
int main(int argc, char **argv)
{
  double ompclockALL = omp_get_wtime();
  std::cout << "TreeConstructor v1.00" << std::endl;
  double ompclock = omp_get_wtime();
  srand(time(NULL));
  t_objects objectlist;
  Node* Tree = NULL;
  Node* Leafs = NULL;
  //generate some random positions from [0,0,0] to [1,1,1]
  //TODO add an function to partition these cube sections and use a 2 level BVH?
  //[first one for unit cubes and second ones inside these cubes when got hit?]
  //or better scale every object with the scenes size?
  std::cout << "creating object list ";ompclock = omp_get_wtime();
  for(int i = (argc >= 2? atoi(argv[1]):STD_NUM_OBJ); i >= 0; --i)
  {
    objectlist.push_back(Object(Vect(dot(rand())/dot(RAND_MAX),dot(rand())/dot(RAND_MAX),dot(rand())/dot(RAND_MAX))));
  }
  std::cout << "took: " << std::setprecision(3) << dot(omp_get_wtime()-ompclock) << "s" << std::endl;
  std::cout << "sorting object list ";ompclock = omp_get_wtime();
  std::sort(objectlist.begin(),objectlist.end(),ObjEval());
  std::cout << "took: " << std::setprecision(3) << dot(omp_get_wtime()-ompclock) << "s" << std::endl;
  
  //SORT BEFORE ALLOC!!! (because objects are being set here already)
  std::cout << "allocate memory for tree and leafs ";ompclock = omp_get_wtime();
  allocateHierarchy(&Tree,&Leafs,objectlist);
  std::cout << "took: " << std::setprecision(3) << dot(omp_get_wtime()-ompclock) << "s" << std::endl;
  
  std::cout << "generate the Hierarchy " << std::endl;ompclock = omp_get_wtime();
  //can be repeatet as often as needed, because it will always use the same memory
  //if the object-count is the same... recursive tree generation has to new/delete every time
  generateHierarchy(objectlist,Tree,Leafs);
  std::cout << "took: " << std::setprecision(3) << dot(omp_get_wtime()-ompclock) << "s" << std::endl;
  
  //TODO Bounding box?
  
  /*
   *Do your searching stuff in here ;p
   */
  
  std::cout << "free the hierarchie memory ";ompclock = omp_get_wtime();
  destructHierarchy(Tree,Leafs);
  std::cout << "took: " << std::setprecision(3) << dot(omp_get_wtime()-ompclock) << "s" << std::endl;
  
  std::cout << "free the list memory ";ompclock = omp_get_wtime();
  objectlist.clear();
  std::vector<Object>().swap(objectlist);
  std::cout << "took: " << std::setprecision(3) << dot(omp_get_wtime()-ompclock) << "s" << std::endl;
  std::cout << "EVERYTHING took: " << std::setprecision(3) << dot(omp_get_wtime()-ompclockALL) << "s" << std::endl;
  return NULL;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Generating the Tree:
void generateHierarchy( t_objects &list,Node* Tree, Node* Leafs)
{
  int internal_num = list.size()-1;
  static int threadnum = omp_get_max_threads();
  static int threadsize = 1024*1024;
  std::cout << "building tree with " << threadsize << " blocksize and " << threadnum << " threads" << std::endl;
// #pragma omp parallel for num_threads(threadnum) schedule(dynamic, threadsize)
  for(int idx = 0; idx < internal_num;++idx)
  {
// #pragma omp critical(printout)
    if(idx%threadsize ==0)
      std::cout << "\rAT " << idx << "/" << internal_num << "  " << std::setprecision(4) << dot(idx)/dot(internal_num)*100.f << "%  " << std::flush;
    generateNode( list, Tree, Leafs, idx);
  }
      std::cout << "\rAT " << internal_num << "/" << internal_num << "  " << std::setprecision(4) << 100.f << "%  " << std::flush;
  std::cout << std::endl;
}

void generateNode( t_objects &list,Node* Tree, Node* Leafs, int idx)
{
  //from where to where will this node go?
  int2 range = determineRange(list, idx);
  //where will be the split within the range?
  int split = findSplit(list, range.x, range.y);
  
  //The lower Nodes/Leafs will be at split position and at split+1 position
  Node* ChA;
  Node* ChB;
  //if the left Child is at the first position of the range, i.e. the
  //split is at the beg of the range, it will be a leaf node to object[X]
  if(split == range.x)
    ChA = &Leafs[split];
  else
    ChA = &Tree[split];
  
  //The same with the right child, if it hits the last index in the range
  //it will be a child, so if the range is only 2 wide [for example from 3 to 4]
  //both childs will be leafs, and if it goes over 3, then one will be a child
  if(split+1 == range.x)
    ChB = &Leafs[split+1];
  else
    ChB = &Tree[split+1];
  Tree[idx].A = ChA;
  Tree[idx].B = ChB;
  //Also set the Parent node
  ChA->parent = &Tree[idx];
  ChB->parent = &Tree[idx];
  
  
}

//Memory management for the Tree:
void allocateHierarchy(Node** Internals, Node** Leafs, t_objects &list)
{
  int lstsize = list.size();
  //to link towards the objects, so one per object
  *Leafs = new Node[lstsize];
  // N-1 Internal nodes for the tree (I guess you know why?)
  *Internals = new Node[lstsize-1];
  //So the loop doesn't have to lookup the list on every iteration
  for(int i = 0; i < lstsize;++i)
  {
    //copy pointer to object i at list position
    (*Leafs)[i].O = list.data() + i;
    list[i].parent = &((*Leafs)[i]);
    //unfortunately you have to live with the ((*Leafs)[i]) syntax because of the double pointer...
    
  }
}

//order is not important but it's always better to stick to a fixed order ^^ better for human-error avoiding and stuff
void destructHierarchy(Node* Internals, Node* Leafs)
{
  if(Leafs != NULL)
    delete[] Leafs;
  if(Internals!=NULL)
    delete[] Internals;
  Leafs = NULL;
  Internals = NULL;
}

//Deeper Hierarchy functions:
int findSplit( t_objects &list, int first, int last)
{
  
    unsigned int firstCode = list[first].mcode;
    unsigned int lastCode = list[last].mcode;
    
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
            unsigned int splitCode = list.at(newSplit).mcode;
            int splitPrefix = CLZ1(firstCode ^ splitCode);
            if (splitPrefix > commonPrefix)
                split = newSplit; // accept proposal
        }
    }
    while (step > 1);

    return split;
}
int2 determineRange(t_objects &list, int index)
{
  //so we don't have to call it every time
  int lso = list.size()-1;
  //tadaah, it's the root node
  if(index == 0)
    return int2( 0, lso);
  //direction to walk to, 1 to the right, -1 to the left
  int dir;
  //morton code diff on the outer known side of our range ... diff mc3 diff mc4 ->DIFF<- [mc5 diff mc6 diff ... ] diff .. 
  int d_min;
  int initialindex = index;
  
  unsigned int minone = list[index-1].mcode;
  unsigned int precis = list[index].mcode;
  unsigned int pluone = list[index+1].mcode;
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
	 
      if(list[index].mcode != list[index+1].mcode)
       //there is a diffrence
	 break;
    }
    //return the end of equal grouped codes
    return int2(initialindex,index);
  }else{
    //Our codes differ, so we seek for the ranges end in the binary search fashion:
    int2 lr= int2(CLZ1(precis ^ minone),CLZ1(precis ^ pluone));
    //now check wich one is higher (codes put side by side and wrote from up to down)
      if(lr.x > lr.y)
      {//to the left, set the search-depth to the right depth
	dir = -1;
	d_min = lr.y;
      }else{//to the right, set the search-depth to the left depth
	dir = 1;
	d_min = lr.x;
      }
    }
    //Now look for an range to search in (power of two)
    int l_max = 2;
    //so we don't have to calc it 3x
    int testindex = index + l_max*dir;
    while((testindex<=lso&&testindex>=0)?(CLZ1(precis ^ list[testindex].mcode)>d_min):(false))
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
            int splitPrefix = CLZ1(precis ^ list.at(newTest).mcode);
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

//Morton related functions:
unsigned int expandBits(unsigned int v)
{
    v = (v * 0x00010001u) & 0xFF0000FFu;
    v = (v * 0x00000101u) & 0x0F00F00Fu;
    v = (v * 0x00000011u) & 0xC30C30C3u;
    v = (v * 0x00000005u) & 0x49249249u;
    return v;
}
unsigned int morton3D(dot x, dot y, dot z)
{
    x = std::min(std::max(x * 1024.0f, 0.0f), 1023.0f);
    y = std::min(std::max(y * 1024.0f, 0.0f), 1023.0f);
    z = std::min(std::max(z * 1024.0f, 0.0f), 1023.0f);
    unsigned int xx = expandBits((unsigned int)x);
    unsigned int yy = expandBits((unsigned int)y);
    unsigned int zz = expandBits((unsigned int)z);
    return xx * 4 + yy * 2 + zz;
}
//Other stuff:
