#include "image.h"

#include <stdlib.h>     /* realloc, free, exit, NULL */


void Image::SetDim(unsigned int w, unsigned int h)
{
  unsigned int newsize = w*h;
  if(size == -1)
  {
    size = newsize;
    n_alloc();
  }
  if(size != newsize)
  {
    size = newsize;
    n_realloc();
  }
}

void Image::n_alloc()
{
  data = (NODE*) malloc(sizeof(NODE)*size);
  for(int i = size - 1; i >= 0;--i)
  {
    data[i].reset();
  }
}

void Image::n_free()
{
  if(data != NULL)
    free(data);
  data = NULL;
  size = -1;
}

void Image::n_realloc()
{
  Color* ndata = (NODE*) realloc(data, sizeof(NODE)*size);
  if(ndata==NULL)
  {
    //do it by hand if it doesn't like the realloc way .-.
    n_free();
    n_alloc();
  }else{
    data = ndata;
    for(int i = size - 1; i >= 0;--i)
    {
      data[i].reset();
    }
  }
}