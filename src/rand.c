#include "global.h"

float rand_float_in_range(float const min, float const max) {
   double const scale = (double)rand() / (double)RAND_MAX;
   return (float)(min + scale * (max - min));
}

int rand_int_in_range(int const min, int const max) {
   int const scale = rand() / RAND_MAX;
   return min + scale * (max-min);
}
