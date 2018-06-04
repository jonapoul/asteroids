#include "global.h"

int main(int argc, char ** argv) {
   gtk_init(&argc, &argv);

   GtkWidget * window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   gtk_window_set_title(       GTK_WINDOW(window), "Asteroids");
   gtk_window_set_position(    GTK_WINDOW(window), GTK_WIN_POS_CENTER);
   gtk_window_set_default_size(GTK_WINDOW(window), VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
   gtk_window_set_type_hint(   GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_UTILITY);
   g_signal_connect(window, "destroy",           G_CALLBACK(gtk_main_quit), NULL);
   g_signal_connect(window, "key-press-event",   G_CALLBACK(on_keydown), NULL);
   g_signal_connect(window, "key-release-event", G_CALLBACK(on_keyup), NULL);

   GtkWidget * area = gtk_gl_area_new();
   gtk_widget_set_vexpand(area, TRUE);
   gtk_widget_set_hexpand(area, TRUE);
   g_signal_connect(area, "realize", G_CALLBACK(on_realise), NULL);
   g_signal_connect(area, "render",  G_CALLBACK(on_render),  NULL);
   gtk_container_add(GTK_CONTAINER(window), area);

   gtk_widget_show_all(window);
   gtk_main();

   return 0;
}
