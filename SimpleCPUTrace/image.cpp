#include "image.h"

#include <stdlib.h>     /* realloc, free, exit, NULL */
#include <fstream>

void Image::SetDim(unsigned int w, unsigned int h)
{
  unsigned int newsize = w*h;
  dim.X = w;
  dim.Y = h;
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
  data = (Color*) malloc(sizeof(Color)*size);
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
  Color* ndata = (Color*) realloc(data, sizeof(Color)*size);
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

inline dot clamp(dot x){ return x<0 ? 0 : x>1 ? 1 : x; } 
inline int toInt(dot x){ return int(pow(clamp(x),1/2.2)*255+.5); } 

bool Image::Save(std::string file)
{
  
  FILE *f = std::fopen(file.c_str(), "w");         // Write image to PPM file. 
  std::fprintf(f, "P3\n%d %d\n%d\n", dim.X, dim.Y, 255); 
  for (int i=0; i<size; i++) 
    std::fprintf(f,"%d %d %d ", toInt(data[i].R), toInt(data[i].G), toInt(data[i].B)); 
  return true;
  
//   std::fstream f;
//   
//   f.open(file.c_str(), std::ios::out | std::ios::trunc);
//   if(!f)
//     return false;//cannot open file for whatever reason...
//     f << "P3\n" << dim.X << " " << dim.Y <<"\n" << 255 << "\n";
//   //   f << "P3\n " << dim.X << " " << dim.Y << std::endl << 255 << std::endl;
//   
//   for(int i = 0; i < size ; ++i)
//     f << data[i].R << " " << data[i].G << " " << data[i].B << " ";
//   
//   f.close();
//   return true;
}

void Image::PutPixel(unsigned int x, unsigned int y, Color col)
{
  if(x>=dim.X || y >= dim.Y)
    return;
  data[x+y*dim.X] = col;
}

void Image::genSample()
{
  for(dot x = 0; x < dim.X; ++x)
  {
    for(dot y = 0; y < dim.Y; ++y)
    {
      Color col;
      col.R = x/dot(dim.X);
      col.G = dot(int(x+y)%256)/256;
      col.B = y/dot(dim.Y);
      PutPixel(x,y,col);
    }
  }
}












