#include "Window.h"

#include <iostream>

using std::cout;
using std::endl;
using std::getchar;
using std::string;

int Window::count = 0;

Window::Window(std::string title, int ScreenID, int w, int h, int x, int y)
{
  //start SDL if first window, also works again after last window closed
  if(count == 0)
  {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
      cout <<  "Could not initiliaze SDL: " << SDL_GetError() << endl;
      getchar();
      return;
    }//SDL_Init
#ifdef _WIN32
    GLenum err;
    if (GLEW_OK != (err = glewInit())){
      cout << "Could not init GLEW: " << glewGetErrorString(err) << endl;
      return false;
    }//glew
#endif
  }//count == 0
  ++count;
  
  unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS;
  
  //some GL stuff ;P
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  //get the sceen size
  SDL_GetDisplayBounds(ScreenID,&pos);
  
  SDL_Rect tempScreen = pos;
  //check if the new size and position is valid
  if(w > 0 && w < pos.w)
  {
    pos.x = pos.w/2 - w/2;
    pos.w = w;
  }
  if(h > 0 && h < pos.h)
  {
    pos.y = pos.h/2 - h/2;
    pos.h = h;
  }
  if(x > 0 && x + pos.w < tempScreen.w)
  {
    pos.x = x;
  }
  if(y > 0 && y + pos.h < tempScreen.h)
  {
    pos.y = y;
  }
  //cause Linux needs it
  if(pos.w == tempScreen.w && pos.h == tempScreen.h && string(SDL_GetPlatform()) == "Linux")
  {
    flags = flags | SDL_WINDOW_FULLSCREEN;
  }
  //and create it *^*
  g_Window = SDL_CreateWindow(title.c_str(), pos.x,pos.y,pos.w,pos.h,flags);
  if (!g_Window){
    cout <<  "Could not initiliaze Window: " << SDL_GetError() << endl;
    getchar();
    return;
  }
  
  // Make the OpenGL 3.2 context
  g_Context = SDL_GL_CreateContext(g_Window);
  if (!g_Context){
    cout <<  "Could not initiliaze Context: " << SDL_GetError() << endl;
    getchar();
    return;
  }
  SDL_GL_SetSwapInterval(1); // VSync
  
  
}//Window::Window

Window::~Window()
{
  --count;
  if(count == 0)
  {
    SDL_Quit();
  }//count == 0
}//Window::~Window


void Window::blit()
{
  SDL_GL_SwapWindow(g_Window);
}

void Window::Window::contextFocus()
{
  SDL_GL_MakeCurrent(g_Window,g_Context);
}

