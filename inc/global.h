#ifndef ASTEROIDS_GLOBAL_H
#define ASTEROIDS_GLOBAL_H

#include <gtk/gtk.h>
#include <epoxy/gl.h>
#include <epoxy/glx.h>
#include <math.h>
#include "DashGL/dashgl.h"

#define VIEWPORT_WIDTH  640
#define VIEWPORT_HEIGHT 480
#define NUM_BULLETS     20
#define NUM_ASTEROIDS   3
#define print_vec3(v)   print_vec3_impl(v,#v)

/* on_realise.c */
void on_realise(GtkGLArea * area);

/* on_render.c */
void on_render(GtkGLArea * area);

/* vec3.c */
float distance(vec3 const v1, vec3 const v2);
void set_vec3(vec3 * v, float const x, float const y, float const z);
void set_colour(vec3 * c, float const r, float const g, float const b);
void print_vec3_impl(vec3 const v, char const * const name);

/* mat4.c */
void mat4_scale(vec3 const in, mat4 out);

/* locations.c */
GLint get_attribute(char const * const name);
GLint get_uniform(char const * const name);

/* events.c */
gboolean on_idle(gpointer * data);
gboolean on_keydown(GtkWidget * widget, GdkEventKey * event);
gboolean on_keyup(GtkWidget * widget, GdkEventKey * event);

/* random.c */
float rand_float_in_range(float const min, float const max);
int rand_int_in_range(int const min, int const max);

/* global variables */
GLuint program;
GLuint vao;
GLint  attribute_coord2d;
GLint  attribute_texcoord;
GLint  uniform_mytexture;
GLint  uniform_mvp;
GLuint vbo_triangle;

/* structs */
typedef struct {
   float angle;
   vec3 pos;
   gboolean active;
   int ticks;
   float speed;
} bullet;

/* struct instances */
struct {
   GLuint vbo;
   GLuint tex;
   mat4 mvp;
   vec3 pos;
   float x_speed;
   float y_speed;
   float max_speed;
   float force;
   float width;
   float height;
   gboolean up;
   gboolean down;
   gboolean left;
   gboolean right;
   gboolean space;
   float angle;
   float angle_speed;

   GLuint bullet_vbo;
   GLuint bullet_tex;
   int num_ticks;
   float bullet_width;
   float bullet_height;
   bullet bullets[NUM_BULLETS];
} player;

typedef struct {
   vec3 pos;
   float angle;
   float x_speed;
   float y_speed;
   float angle_speed;
   float size;
   gboolean active;
} rock;

struct {
   GLuint vbo;
   GLuint tex;
   mat4 mvp;
   float radius;
   rock rocks[NUM_ASTEROIDS];
} asteroid;

#endif
