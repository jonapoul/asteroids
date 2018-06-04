#include "global.h"

/* should be in dashgl? */
void mat4_scale(vec3 const in, mat4 out) {
   out[M4_00] = in[0];
   out[M4_01] = 0.0f;
   out[M4_02] = 0.0f;
   out[M4_03] = 0.0f;
   out[M4_10] = 0.0f;
   out[M4_11] = in[1];
   out[M4_12] = 0.0f;
   out[M4_13] = 0.0f;
   out[M4_20] = 0.0f;
   out[M4_21] = 0.0f;
   out[M4_22] = in[2];
   out[M4_23] = 0.0f;
   out[M4_30] = 0.0f;
   out[M4_31] = 0.0f;
   out[M4_32] = 0.0f;
   out[M4_33] = 1.0f;
}
