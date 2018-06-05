#include "global.h"

void set_vec3(vec3 * v, float const x, float const y, float const z) {
   (*v)[0] = x;
   (*v)[1] = y;
   (*v)[2] = z;
}

void set_colour(vec3 * c, float const r, float const g, float const b) {
   (*c)[0] = r / 255.f;
   (*c)[1] = g / 255.f;
   (*c)[2] = b / 255.f;
}

void print_vec3_impl(vec3 const v, char const * const name) {
   g_print("%s = (%6.2f %6.2f %6.2f)\n", name, v[0], v[1], v[2]);
}

float distance(vec3 const v1, vec3 const v2) {
   return sqrt(  powf(v1[0]-v2[0], 2.f)
               + powf(v1[1]-v2[1], 2.f)
               + powf(v1[2]-v2[2], 2.f));
}
