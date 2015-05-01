// #pragma message("NEKO_DECL_H")


#include <algorithm>

#ifndef NEKO_DECL_H
#define NEKO_DECL_H

typedef float dot;

template<class T>
inline
T min(T const &a, T const &b){return a<b?a:b;}
template<class T>
inline
T max(T const &a, T const &b){return a<b?b:a;}

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

//Morton related functions:

unsigned int expandBits(unsigned int v);

unsigned int morton3D(dot x, dot y, dot z);

#endif