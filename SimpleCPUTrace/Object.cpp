#include "Object.h"

#include <fstream>


inline
std::string cutspace(std::string in)
{
  while(in.front() == ' ')
    in = in.substr(1);
  while(in.back() == ' ')
    in.pop_back();
  return in;
}

Vect extract(std::string v)
{
  Vect ret;
  std::string tmp;
  if(v.find(',') == std::string::npos)
    return Vect();//wrong input .-.
    tmp = cutspace(v.substr(0,v.find(',')));
  v = v.substr(v.find(',')+1);
  if(tmp.empty())
    tmp = "0";//because of reason (1,,) or (2,,3) n stuff
    ret.X = atof(tmp.c_str());
  if(v.find(',') == std::string::npos)
    return Vect();//wrong input .-.
    tmp = cutspace(v.substr(0,v.find(',')));
  v = v.substr(v.find(',')+1);
  if(tmp.empty())
    tmp = "0";
  ret.Y = atof(tmp.c_str());
  //last number, so no ','
  tmp = cutspace(v);
  if(tmp.empty())
    tmp = "0";
  ret.Z = atof(tmp.c_str());
  return ret;
}

void Object::Clear()
{
  while(!tris.empty())
  {
    if(visualizerLST!=NULL)
      visualizerLST->UnRegister(&tris.back());
    tris.pop_back();
  }
}

bool Object::load(std::string file)
  {
    if(!tris.empty())
      Clear();
    
    std::fstream f;
    f.open(file,std::ios::in);
    if(!f)
      return false;//error opening File
      while(!f.eof())
      {
	std::string line;
	std::getline(f,line);
	
	if(line.empty() || line.find('#') != std::string::npos)
	{
	  continue;
	}
	Vect A,B,C;
	if(line.find('(') == std::string::npos)
	  continue;//invalid line...
	  line = line.substr(line.find('('));
	if(line.find(')') == std::string::npos)
	  continue;//invalid line...
	  std::string sA = line.substr(1,line.find(')')-1);
	A = extract(sA);
	
	line = line.substr(1);
	if(line.find('(') == std::string::npos)
	  continue;//invalid line...
	  line = line.substr(line.find('('));
	if(line.find(')') == std::string::npos)
	  continue;//invalid line...
	  std::string sB = line.substr(1,line.find(')')-1);
	B = extract(sB);
	
	
	line = line.substr(1);
	if(line.find('(') == std::string::npos)
	  continue;//invalid line...
	  line = line.substr(line.find('('));
	if(line.find(')') == std::string::npos)
	  continue;//invalid line...
	  std::string sC = line.substr(1,line.find(')')-1);
	C = extract(sC);
	
	tris.push_back(Tri(A,B,C));
	
      }
      
      f.close();
      
      if(!tris.empty())
	for(std::list<Tri>::iterator it = tris.begin(); it != tris.end(); ++it)
	{
	  visualizerLST->Register(&(*it));
	}
  }