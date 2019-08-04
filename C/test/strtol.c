/* strtol example */
#include <stdio.h>      /* printf */
#include <stdlib.h>     /* strtol */

int main ()
{
  char szNumbers[] = "1000";
  char * pEnd;
  long int longint;
  longint = strtol (szNumbers, NULL, 16);
  printf ("The decimal equivalents are: %ld.\n", longint);
  return 0;
}
