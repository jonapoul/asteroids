#include "global.h"

static void attrib_pointers() {
   glVertexAttribPointer(attribute_coord2d,
                         2,
                         GL_FLOAT,
                         GL_FALSE,
                         4*sizeof(float),
                         0);
   glVertexAttribPointer(attribute_texcoord,
                         2,
                         GL_FLOAT,
                         GL_FALSE,
                         4*sizeof(float),
                         (void*)(sizeof(float)*2));
}

void on_render(GtkGLArea * area) {
   /* reset canvas */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glUseProgram(program);

   /* enable drawing */
   glBindVertexArray(vao);
   glEnableVertexAttribArray(attribute_coord2d);
   glEnableVertexAttribArray(attribute_texcoord);

   /* draw player */
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, player.tex);
   glUniform1i(uniform_mytexture, 0);
   glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, player.mvp);
   glBindBuffer(GL_ARRAY_BUFFER, player.vbo);
   attrib_pointers();
   glDrawArrays(GL_TRIANGLES, 0, 6);

   /* disable drawing */
   glDisableVertexAttribArray(attribute_coord2d);
   glDisableVertexAttribArray(attribute_texcoord);
}
