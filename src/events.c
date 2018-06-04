#include "global.h"

gboolean on_idle(gpointer * data) {
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

   /* activate bullets */
   for (int i = 0; i < MAX_BULLETS; ++i) {
      if (player.bullets[i].active == FALSE) {
         continue;
      }
      if (player.bullets[i].ticks < 0) {
         player.bullets[i].active = FALSE;
      }
      float const dx = sin(player.bullets[i].angle) * player.bullets[i].speed;
      float const dy = cos(player.bullets[i].angle) * player.bullets[i].speed;
      player.bullets[i].pos[0] -= dx;
      player.bullets[i].pos[1] += dy;
      player.bullets[i].ticks--;
   }

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
         for (int i = 0; player.space == FALSE && i < MAX_BULLETS; ++i) {
            if (player.bullets[i].active) {
               continue;
            }
            player.bullets[i].active = TRUE;
            player.bullets[i].angle = player.angle;
            player.bullets[i].ticks = player.num_ticks;
            copy_vec3(&player.bullets[i].pos, player.pos);
            break;
         }
         player.space = TRUE;
         break;

      /* quit */
      case GDK_KEY_Escape:
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
