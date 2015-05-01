#include "Scene.h"

#include <iostream>
#include <stdio.h>      /* printf, scanf, puts */
#include <stdlib.h>     /* realloc, free, exit, NULL */
using namespace std;

int main(int argc, char **argv)
{
//   int input,n;
//   int count = 0;
//   int* numbers = NULL;
//   int* more_numbers = NULL;
// 
//   do {
//      printf ("Enter an integer value (0 to end): ");
//      scanf ("%d", &input);
//      count++;
// 
//      more_numbers = (int*) realloc (numbers, count * sizeof(int));
// 
//      if (more_numbers!=NULL) {
//        numbers=more_numbers;
//        numbers[count-1]=input;
//      }
//      else {
//        free (numbers);
//        puts ("Error (re)allocating memory");
//        exit (1);
//      }
//   } while (input!=0);
// 
//   printf ("Numbers entered: ");
//   for (n=0;n<count;n++) printf ("%d ",numbers[n]);
//   free (numbers);
  
  
  Scene scn;
  
  scn.load("sample.obj.neko");
//   std::getchar();
  
  return 0;
}