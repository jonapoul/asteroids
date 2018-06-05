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

static void mirror_player() {
   /* check if its necessary */
   gboolean draw_mirror = FALSE;
   if (player.pos[0] > VIEWPORT_WIDTH - player.width) {
      player.pos[0] -= VIEWPORT_WIDTH;
      draw_mirror = TRUE;
   } else if (player.pos[0] < player.width) {
      player.pos[0] += VIEWPORT_WIDTH;
      draw_mirror = TRUE;
   }

   if (player.pos[1] > VIEWPORT_HEIGHT - player.height) {
      player.pos[1] -= VIEWPORT_HEIGHT;
      draw_mirror = TRUE;
   } else if (player.pos[1] < player.height) {
      player.pos[1] += VIEWPORT_HEIGHT;
      draw_mirror = TRUE;
   }

   /* if so, draw the ship coming out the other end of the screen */
   if (draw_mirror) {
      mat4 pos, rot;
      mat4_translate(player.pos, pos);
      mat4_rotate_z(player.angle, rot);
      mat4_multiply(pos, rot, player.mvp);
      glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, player.mvp);
      glBindBuffer(GL_ARRAY_BUFFER, player.vbo);
      attrib_pointers();
      glDrawArrays(GL_TRIANGLES, 0, 6);
   }
}

static void mirror_bullet(int const i) {
   gboolean draw_mirror = FALSE;
   if (player.bullets[i].pos[0] > VIEWPORT_WIDTH - player.bullet_width) {
      player.bullets[i].pos[0] -= VIEWPORT_WIDTH;
      draw_mirror = TRUE;
   } else if (player.bullets[i].pos[0] < player.bullet_width) {
      player.bullets[i].pos[0] += VIEWPORT_WIDTH;
      draw_mirror = TRUE;
   }

   if (player.bullets[i].pos[1] > VIEWPORT_HEIGHT - player.bullet_height) {
      player.bullets[i].pos[1] -= VIEWPORT_HEIGHT;
      draw_mirror = TRUE;
   } else if (player.bullets[i].pos[1] < player.bullet_height) {
      player.bullets[i].pos[1] += VIEWPORT_HEIGHT;
      draw_mirror = TRUE;
   }
   if (draw_mirror) {
      mat4 pos, rot;
      mat4_translate(player.bullets[i].pos, pos);
      mat4_rotate_z(player.bullets[i].angle, rot);
      mat4_multiply(pos, rot, player.mvp);
      glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, player.mvp);
      glBindBuffer(GL_ARRAY_BUFFER, player.bullet_vbo);
      attrib_pointers();
      glDrawArrays(GL_TRIANGLES, 0, 6);
   }
}

static void mirror_asteroid(int const i) {
   rock * const r = &(asteroid.rocks[i]);
   /* check if we need to mirror */
   gboolean draw_mirror = FALSE;
   float const size = r->scale * asteroid.radius;
   if (r->pos[0] > VIEWPORT_WIDTH - size) {
      r->pos[0] -= VIEWPORT_WIDTH;
      draw_mirror = TRUE;
   } else if (r->pos[0] < size) {
      r->pos[0] += VIEWPORT_WIDTH;
      draw_mirror = TRUE;
   }
   if (r->pos[1] > VIEWPORT_HEIGHT - size) {
      r->pos[1] -= VIEWPORT_HEIGHT;
      draw_mirror = TRUE;
   } else if (r->pos[1] < size) {
      r->pos[1] += VIEWPORT_HEIGHT;
      draw_mirror = TRUE;
   }
   /* do the mirroring */
   if (draw_mirror) {
      mat4 pos, rot;
      mat4_translate(r->pos, pos);
      mat4_rotate_z(r->angle, rot);
      mat4_multiply(pos, rot, asteroid.mvp);
      glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, asteroid.mvp);
      glDrawArrays(GL_TRIANGLES, 0, 6);
   }
}

void on_render(GtkGLArea * area) {
   g_print("input = (%d %d %d %d) angle = %6.2f dx = %6.2f dy = %6.2f sizes = (%.2f %.2f %.2f)\r",
           player.up, player.down, player.left, player.right,
           player.angle, player.x_speed, player.y_speed,
           asteroid.rocks[0].scale, asteroid.rocks[1].scale, asteroid.rocks[2].scale);
   /* reset canvas */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glUseProgram(program);

   /* enable drawing */
   glBindVertexArray(vao);
   glEnableVertexAttribArray(attribute_coord2d);
   glEnableVertexAttribArray(attribute_texcoord);

   /* draw bullets */
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, player.bullet_tex);
   glUniform1i(uniform_mytexture, 0);
   for (int i = 0; i < NUM_BULLETS; ++i) {
      if (player.bullets[i].active == FALSE) {
         continue;
      }
      mat4 pos, rot;
      mat4_translate(player.bullets[i].pos, pos);
      mat4_rotate_z(player.bullets[i].angle, rot);
      mat4_multiply(pos, rot, player.mvp);
      glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, player.mvp);
      glBindBuffer(GL_ARRAY_BUFFER, player.bullet_vbo);
      attrib_pointers();
      glDrawArrays(GL_TRIANGLES, 0, 6);
      mirror_bullet(i);
   }

   /* draw player */
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, player.tex);
   glUniform1i(uniform_mytexture, 0);
   mat4 pos, rot;
   mat4_translate(player.pos, pos);
   mat4_rotate_z(player.angle, rot);
   mat4_multiply(pos, rot, player.mvp);
   glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, player.mvp);
   glBindBuffer(GL_ARRAY_BUFFER, player.vbo);
   attrib_pointers();
   glDrawArrays(GL_TRIANGLES, 0, 6);
   mirror_player();

   /* draw asteroids */
   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, asteroid.tex);
   glUniform1i(uniform_mytexture, 0);
   for (int i = 0; i < NUM_ASTEROIDS; ++i) {
      rock * const r = &(asteroid.rocks[i]);
      if (r->active == FALSE) {
         continue;
      }
      vec3 scale_vec;
      set_vec3(&scale_vec, r->scale, r->scale, 0.0);
      mat4 scale, pos, rot;
      mat4_scale(scale_vec, scale);
      mat4_translate(r->pos, pos);
      mat4_rotate_z(r->angle, rot);
      mat4_multiply(pos, rot, asteroid.mvp);
      mat4_multiply(asteroid.mvp, scale, asteroid.mvp);
      glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, asteroid.mvp);
      glBindBuffer(GL_ARRAY_BUFFER, asteroid.vbo);
      attrib_pointers();
      glDrawArrays(GL_TRIANGLES, 0, 6);
      mirror_asteroid(i);
   }

   /* disable drawing */
   glDisableVertexAttribArray(attribute_coord2d);
   glDisableVertexAttribArray(attribute_texcoord);
}
