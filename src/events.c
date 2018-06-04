#include "global.h"

static void keep_within_bounds(float const min, float const max, float * val) {
   if (*val < min) *val = min;
   if (*val > max) *val = max;
}

gboolean on_idle(gpointer * data) {
   /* rotate player */
   if (player.right) player.angle -= player.angle_speed;
   if (player.left)  player.angle += player.angle_speed;

   /* accelerate player */
   if (player.up) {
      player.x_speed += player.force * sin(player.angle);
      player.y_speed += player.force * cos(player.angle);
   }
   if      (player.x_speed < -player.max_speed) player.x_speed = -player.max_speed;
   else if (player.x_speed >  player.max_speed) player.x_speed =  player.max_speed;
   if      (player.y_speed < -player.max_speed) player.y_speed = -player.max_speed;
   else if (player.y_speed >  player.max_speed) player.y_speed =  player.max_speed;

   /* move player */
   player.pos[0] -= player.x_speed;
   player.pos[1] += player.y_speed;
   if (player.pos[0] > VIEWPORT_WIDTH) {
      player.pos[0]    = VIEWPORT_WIDTH;
      player.x_speed   = 0.0;
   } else if (player.pos[0] < 0) {
      player.pos[0]    = 0.0;
      player.x_speed   = 0.0;
   }
   if (player.pos[1] > VIEWPORT_HEIGHT) {
      player.pos[1]    = VIEWPORT_HEIGHT;
      player.y_speed   = 0.0;
   } else if (player.pos[1] < 0) {
      player.pos[1]    = 0.0;
      player.y_speed   = 0.0;
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
      case GDK_KEY_Left:
         player.left = TRUE;
         break;
      case GDK_KEY_Right:
         player.right = TRUE;
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
      case GDK_KEY_Left:
         player.left = FALSE;
         break;
      case GDK_KEY_Right:
         player.right = FALSE;
         break;
      case GDK_KEY_space:
         // player.space = FALSE;
         break;
   }
   return TRUE;
}
