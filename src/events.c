#include "global.h"

static void move_player() {
   /* rotate player */
   if (player.right) player.angle -= player.angle_speed;
   if (player.left)  player.angle += player.angle_speed;

   /* accelerate player */
   if (player.up) {
      player.x_speed += player.force * sin(player.angle);
      player.y_speed += player.force * cos(player.angle);
   }
   if (player.down) {
      player.x_speed -= player.force * sin(player.angle);
      player.y_speed -= player.force * cos(player.angle);
   }
   if      (player.x_speed < -player.max_speed) player.x_speed = -player.max_speed;
   else if (player.x_speed >  player.max_speed) player.x_speed =  player.max_speed;
   if      (player.y_speed < -player.max_speed) player.y_speed = -player.max_speed;
   else if (player.y_speed >  player.max_speed) player.y_speed =  player.max_speed;

   /* move player */
   player.pos[0] -= player.x_speed;
   player.pos[1] += player.y_speed;

}

static void move_asteroids() {
   for (int i = 0; i < NUM_ASTEROIDS; ++i) {
      rock * const r = &(asteroid.rocks[i]);
      if (r->active == FALSE) {
         continue;
      }
      r->pos[0] += r->x_speed;
      r->pos[1] += r->y_speed;
      r->angle  += r->angle_speed;
   }
}

gboolean on_idle(gpointer * data) {
   /* activate and move bullets */
   int split = 0;
   int index = 0;
   for (int i = 0; i < NUM_BULLETS; ++i) {
      bullet * const b = &(player.bullets[i]);
      if (b->active == FALSE) {
         continue;
      }
      if (b->ticks < 0) {
         b->active = FALSE;
      }
      float const dx = sin(b->angle) * b->speed;
      float const dy = cos(b->angle) * b->speed;
      b->pos[0] -= dx;
      b->pos[1] += dy;
      b->ticks--;
      for (int j = 0; j < NUM_ASTEROIDS; ++j) {
         rock * const r = &(asteroid.rocks[j]);
         if (r->active == FALSE) {
            continue;
         }
         if (distance(b->pos, r->pos) < asteroid.radius * r->scale) {
            r->active = FALSE;
            b->active = FALSE;
            if (r->scale < 0.5) {
               split = 2;
               index = j;
            }
            break;
         }
      }
      if (!split) {
         continue;
      }
      for (int j = 0; j < NUM_ASTEROIDS; ++j) {
         rock * const r_j     = &(asteroid.rocks[j]);
         rock * const r_index = &(asteroid.rocks[index]);
         if (j == index || r_j->active) {
            continue;
         }
         r_j->active = TRUE;
         r_j->scale  = r_index->scale / 2.0;
         r_j->angle  = rand_float_in_range(0.0, 2*M_PI);
         vec3_copy(r_index->pos, r_j->pos);
         switch (split) {
            case 2:
               r_j->x_speed = r_index->x_speed *  1.2f;
               r_j->y_speed = r_index->y_speed * -1.2f;
               break;
            case 1:
               r_j->x_speed = r_index->x_speed * -1.2f;
               r_j->y_speed = r_index->y_speed *  1.2f;
               break;
         }
         split--;
         if (!split) {
            break;
         }
      }
   }
   move_player();
   move_asteroids();

   gtk_widget_queue_draw(GTK_WIDGET(data));
   return TRUE;
}

gboolean on_keydown(GtkWidget * widget, GdkEventKey * event) {
   switch (event->keyval) {
      /* move */
      case GDK_KEY_Up:
         player.up = TRUE;
         break;
      case GDK_KEY_Down:
         player.down = TRUE;
         break;
      case GDK_KEY_Left:
         player.left = TRUE;
         break;
      case GDK_KEY_Right:
         player.right = TRUE;
         break;

      /* shoot */
      case GDK_KEY_space:
         for (int i = 0; player.space == FALSE && i < NUM_BULLETS; ++i) {
            bullet * const b = &(player.bullets[i]);
            if (b->active) {
               continue;
            }
            b->active = TRUE;
            b->angle = player.angle;
            b->ticks = player.num_ticks;
            vec3_copy(player.pos, b->pos);
            break;
         }
         player.space = TRUE;
         break;

      /* quit */
      case GDK_KEY_Escape:
         g_print("\n");
         exit(1);
         break;
   }
   return TRUE;
}

gboolean on_keyup(GtkWidget * widget, GdkEventKey * event) {
   switch (event->keyval) {
      case GDK_KEY_Up:
         player.up = FALSE;
         break;
      case GDK_KEY_Down:
         player.down = FALSE;
         break;
      case GDK_KEY_Left:
         player.left = FALSE;
         break;
      case GDK_KEY_Right:
         player.right = FALSE;
         break;
      case GDK_KEY_space:
         player.space = FALSE;
         break;
   }
   return TRUE;
}
