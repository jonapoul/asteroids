#include "global.h"

GLint get_attribute(char const * const name) {
   GLint attribute = glGetAttribLocation(program, name);
   if (attribute == -1) {
      fprintf(stderr, "Could not bind attribute '%s'\n", name);
      exit(1);
   }
   return attribute;
}

GLint get_uniform(char const * const name) {
   GLint uniform = glGetUniformLocation(program, name);
   if (uniform == -1) {
      fprintf(stderr, "Could not bind uniform '%s'\n", name);
      exit(1);
   }
   return uniform;
}
