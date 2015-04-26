#include "Window.h"

#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char **argv)
{
  cout << "SDL Window v0.10" << endl;
  
  Window* win = new Window("Titel",0,500,200);
  
  win->blit();
  /*
   * load your stuff in here after blitting a nice image and remember to
   * check events so the window won't be marked as "not responding"
   */
  SDL_Delay(1000);
  
  delete win;
  
  
  Window* winb = new Window("Titel");
  
  winb->blit();
  
  /*
   * place your game loop here or so ;P
   * 
   */
  
  SDL_Delay(5000);
  
  delete winb;
  
  return NULL;
}//main