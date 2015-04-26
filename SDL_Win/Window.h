#include <SDL2/SDL.h>

#include <string>


#ifdef _WIN32
#	pragma comment(lib, "opengl32.lib")
#	pragma comment(lib, "sdl.lib")
#	pragma comment(lib, "sdlmain.lib")
#	define GLEW_STATIC 1
#	include "GL/GLEW.h"
#else
#	define GL_GLEXT_PROTOTYPES
#	include "SDL2/SDL_opengl.h"
#endif

#ifndef NEKO_WINDOW_H
#define NEKO_WINDOW_H

#ifndef NEKO_DOT
#define NEKO_DOT
typedef float dot;
#endif

class Window
{
public:
  //ScreenID is the screen index (leave 0 for standart screen), set your values, set -1 if you want default
  Window(std::string title = "NekoWin", int ScreenID = 0, int w = -1, int h = -1, int x = -1, int y = -1);
  ~Window();
  //show your renders
  void blit();
  //focus this window and it's context
  void contextFocus();
  
private:
  SDL_Rect pos;
  //how many windows are open( for automatic SDL init and Quit)
  static int count;
  
  //handles
  SDL_Window* g_Window;
  SDL_GLContext g_Context;
  
};//then Window itself

#endif