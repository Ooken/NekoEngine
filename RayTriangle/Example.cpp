#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

typedef float dot;

struct Vect
{
  dot X;
  dot Y;
  dot Z;
  
  Vect(dot x=0,dot y = 0,dot z = 0):X(x),Y(y),Z(z){}
  
  Vect operator +(Vect const &v){return Vect(X+v.X,Y+v.Y,Z+v.Z);}
  Vect operator -(Vect const &v){return Vect(X-v.X,Y-v.Y,Z-v.Z);}
  Vect operator /(dot const &v){return Vect(X/v,Y/v,Z/v);}
  Vect operator *(dot const &v){return Vect(X*v,Y*v,Z*v);}
  //Dot Product
  dot operator *(Vect const &v){return X*v.X+Y*v.Y+Z*v.Z;}
  //Cross Product
  Vect cross(Vect const &v)
  {
    return Vect(Y*v.Z-Z*v.Y,
		Z*v.X-X*v.Z,
		X*v.Y-Y*v.X);
  }
  dot len(){return sqrt((*this)*(*this));}
  Vect Normalize(){return (*this)/len();}
};



struct Triangle
{
  Vect A,B,C;
  Vect normal;
  Triangle(Vect t_A=Vect(),Vect t_B=Vect(),Vect t_C=Vect()):A(t_A),B(t_B),C(t_C){setNormal();}
  //void update(Vect t_A=A,Vect t_B=B,Vect t_C=C){A=t_A;B=t_B;C=t_C;normal = (A-B).cross(A-C);normal = normal * normal.len();}
  void setNormal(){normal = (A-B).cross(A-C);normal = normal / normal.len();}
};

struct Ray
{
  Vect Pos;
  Vect Dir;
  Ray(Vect p = Vect(), Vect d = Vect()):Pos(p),Dir(d){}
};


//Returns Distance prom r.Pos to intersection and sets u and v to the coordinates on the triangle
//it assumes that the triangle normals are already calculated
dot IntersectTri(Ray &r, Triangle &Tri, dot *u, dot *v)
{
  //set variables
  Vect a = Tri.A;
  Vect b = Tri.B-Tri.A;
  Vect c = Tri.C-Tri.A;
  Vect n = c.cross(b).Normalize();
  if(n.X==0&&n.Y==0&&n.Z==0)
    return -1;//kein gutes Dreieck ^^"
  Vect w = (r.Pos+r.Dir)-a;
  //precalc the distance
  dot diver = n * r.Dir;
  //plane and ray parallel or stuff
  if(diver == 0)
    return -1;
  dot dist = (n*(a-r.Pos))/diver;
  if(dist <= 0)
    return -1;
  
  
  //some precalculations (dot producs...)
  dot bb = b*b;
  dot cc = c*c;
  dot bc = b*c;
  dot wb = w*b;
  dot wc = w*c;
  
  //reuse the var for the UV calculation
  diver = (bc*bc)-(bb*cc);
  *u = ((bc*wc)-(cc*wb))/diver;
  if(*u < 0 || *u > 1)
    return -1;//is far way to far away
  *v = ((bc*wb)-(bb*wc))/diver;
  if(*v < 0 || (*v + *u) > 1)
    return -1;//and again... to far away...
  //done
  return dist;
}

void Display(Vect v)
{
  cout << "[ " << setw(7) << setprecision(4) << v.X << " | " << setw(7) << setprecision(4) << v.Y << " | " << setw(7) << setprecision(4) << v.Z << " ]" << setw(7) << setprecision(4) << v.len() << endl;
}

int main(int argc, char **argv)
{
  cout << "Ray-Triangle intersection test" << endl;
  Triangle tr(Vect(0,0,0),Vect(1,0,0),Vect(0,1,0));
  Ray r(Vect(-0.3,0.2,1),Vect(0.5,0,-1).Normalize());
  Display(tr.A-tr.B);
  Display(tr.A-tr.C);
  Display(tr.B-tr.C);
  Display(tr.normal);
  cout << "----- ----- ----- " << endl;
  Display(r.Pos);
  Display(r.Dir);
  cout << "----- ----- ----- " << endl;
  dot u=0;
  dot v=0;
  dot d = IntersectTri(r,tr,&u,&v);
  Vect res(u,v,d);
  Display(res);
//   set the ray to miss the triangle, for testing
  cout << "----- ----- ----- "<< "----- ----- ----- " << endl;
  r.Dir.Y = 3;
  Display(r.Pos);
  Display(r.Dir);
  cout << "----- ----- ----- " << endl;
  u=0;
  v=0;
  d = IntersectTri(r,tr,&u,&v);
  res = Vect(u,v,d);
  Display(res);
  return NULL;
}