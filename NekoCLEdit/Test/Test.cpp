#include <iostream>

#include <ncurses.h>

using namespace std;

int main(int argc, char **argv)
{
  initscr();                 /* Start curses mode     */
  printw("Hello World !!!"); /* Print Hello World    */
  refresh();                 /* Print it on to the real screen */
  getch();                   /* Wait for user input */
  endwin();                  /* End curses mode    */
  return 0;
}
