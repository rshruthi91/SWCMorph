#ifndef COMMON
#define COMMON

#include <cmath>
#include <stdlib.h>

# define PI 3.14159265358979323846  /* pi */

//Inline this function to avoid multiple definitions
inline double absdiff(double a, double b) {
  if(a>b) return a-b;
  else return b-a;
}

#endif // COMMON

