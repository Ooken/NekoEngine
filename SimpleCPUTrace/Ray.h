// #pragma message("NEKO_RAY_H")
#ifndef NEKO_RAY_H
#define NEKO_RAY_H

#include "Vect.h"

struct Ray
{
  Vect Pos;
  Vect Dir;
  Vect invDir;
  Ray(Vect p = Vect(), Vect d = Vect()):Pos(p),Dir(d){invDir = Dir.OneDiv(); }
};

#endif