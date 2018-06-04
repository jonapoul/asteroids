#include "global.h"

static void keep_within_bounds(float const min, float const max, float * val) {
   if (*val < min) *val = min;
   if (*val > max) *val = max;
}

gboolean on_idle(gpointer * data) {
   gtk_widget_queue_draw(GTK_WIDGET(data));
   return TRUE;
}

gboolean on_keydown(GtkWidget * widget, GdkEventKey * event) {
   switch (event->keyval) {
      /* quit */
      case GDK_KEY_Escape:
         exit(1);
         break;
   }
   return TRUE;
}

gboolean on_keyup(GtkWidget * widget, GdkEventKey * event) {
   // switch (event->keyval) {
   //    case GDK_KEY_Left:
   //       player.left = FALSE;
   //       break;
   //    case GDK_KEY_Right:
   //       player.right = FALSE;
   //       break;
   //    case GDK_KEY_space:
   //       player.space = FALSE;
   //       break;
   // }
   return TRUE;
}
