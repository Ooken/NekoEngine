// #pragma message("NEKO_DECL_H")

#ifndef NEKO_DECL_H
#define NEKO_DECL_H

typedef float dot;

template<class T>
inline
T min(T const &a, T const &b){return a<b?a:b;}
template<class T>
inline
T max(T const &a, T const &b){return a<b?b:a;}

#endif