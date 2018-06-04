#include "global.h"

static void init_player() {
   player.width = player.height = 24.0;
   GLfloat const player_vertices[] = {
      -player.width, -player.height, 0.0, 0.0,
      -player.width,  player.height, 0.0, 1.0,
       player.width,  player.height, 1.0, 1.0,
      -player.width, -player.height, 0.0, 0.0,
       player.width,  player.height, 1.0, 1.0,
       player.width, -player.height, 1.0, 0.0,
   };
   glGenBuffers(1, &player.vbo);
   glBindBuffer(GL_ARRAY_BUFFER, player.vbo);
   glBufferData(GL_ARRAY_BUFFER,
                sizeof(player_vertices),
                player_vertices,
                GL_STATIC_DRAW);
   set_vec3(&player.pos,  VIEWPORT_WIDTH/2.f, VIEWPORT_HEIGHT/2.f, 0.f);
   mat4_translate(player.pos, player.mvp);
   player.tex         = shader_load_texture("sprites/player.png");
   player.up          = FALSE;
   player.down        = FALSE;
   player.left        = FALSE;
   player.right       = FALSE;
   player.space       = FALSE;
   player.angle       = 0.0;
   player.x_speed     = 0.0;
   player.y_speed     = 0.0;
   player.force       = 0.1;
   player.max_speed   = 8.0;
   player.angle_speed = 0.05;
}

static void init_bullets() {
   player.bullet_width  = 10.0;
   player.bullet_height = 10.0;
   GLfloat const bullet_vertices[] = {
      -player.bullet_width, -player.bullet_height, 0.0, 0.0,
      -player.bullet_width,  player.bullet_height, 0.0, 1.0,
       player.bullet_width,  player.bullet_height, 1.0, 1.0,

      -player.bullet_width, -player.bullet_height, 0.0, 0.0,
       player.bullet_width,  player.bullet_height, 1.0, 1.0,
       player.bullet_width, -player.bullet_height, 1.0, 0.0
   };

   glGenBuffers(1, &player.bullet_vbo);
   glBindBuffer(GL_ARRAY_BUFFER, player.bullet_vbo);
   glBufferData(GL_ARRAY_BUFFER,
                sizeof(bullet_vertices),
                bullet_vertices,
                GL_STATIC_DRAW);
   player.bullet_tex   = shader_load_texture("sprites/bullet.png");
   player.num_ticks    = 70;
   for (int i = 0; i < NUM_BULLETS; i++) {
      player.bullets[i].active = FALSE;
      player.bullets[i].speed  = 10.0;
   }
}

static void init_asteroid() {
   asteroid.radius = 40.0;
   GLfloat const asteroid_vertices[] = {
      -asteroid.radius, -asteroid.radius, 0.0, 0.0,
      -asteroid.radius,  asteroid.radius, 0.0, 1.0,
       asteroid.radius,  asteroid.radius, 1.0, 1.0,

      -asteroid.radius, -asteroid.radius, 0.0, 0.0,
       asteroid.radius,  asteroid.radius, 1.0, 1.0,
       asteroid.radius, -asteroid.radius, 1.0, 0.0
   };
   glGenBuffers(1, &asteroid.vbo);
   glBindBuffer(GL_ARRAY_BUFFER, asteroid.vbo);
   glBufferData(GL_ARRAY_BUFFER,
                sizeof(asteroid_vertices),
                asteroid_vertices,
                GL_STATIC_DRAW);
   asteroid.tex = shader_load_texture("sprites/asteroid.png");
   for (int i = 0; i < NUM_ASTEROIDS; ++i) {
      rock * const r = &(asteroid.rocks[i]);
      r->active = TRUE;
      set_vec3(&(r->pos),
               rand_float_in_range(0.0, VIEWPORT_WIDTH),
               rand_float_in_range(0.0, VIEWPORT_WIDTH),
               0.0);
      r->angle       = rand_float_in_range( 0.0, 2*M_PI);
      r->x_speed     = rand_float_in_range(-2.0, 2.0);
      r->y_speed     = rand_float_in_range(-2.0, 2.0);
      r->angle_speed = rand_float_in_range(-0.1, 0.1);
      r->size        = rand_float_in_range( 0.2, 3.0); // relative to r->radius
   }
}

void on_realise(GtkGLArea * area) {
   gtk_gl_area_make_current(area);
   GError * err = gtk_gl_area_get_error(area);
   if (err != NULL) {
      fprintf(stderr, "on_realise error: '%s'\n", err->message);
      exit(1);
   }

   printf("Shader         = '%s'\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
   printf("Renderer       = '%s'\n", glGetString(GL_RENDER));
   printf("OpenGL version = '%s'\n", glGetString(GL_VERSION));

   float const bg = 0.f;
   glClearColor(bg, bg, bg, 1.0f);

   glGenVertexArrays(1, &vao);
   glBindVertexArray(vao);

   init_player();
   init_bullets();
   init_asteroid();

   /* load program */
   program = shader_load_program("shader/vertex.glsl",
                                 "shader/fragment.glsl");

   /* get locations of shader variables */
   attribute_coord2d   = get_attribute("coord2d");
   attribute_texcoord  = get_attribute("uv_coord");
   uniform_mytexture   = get_uniform("mytexture");
   uniform_mvp         = get_uniform("mvp");
   GLint uniform_ortho = get_uniform("orthograph");

   glUseProgram(program);

   mat4 orthograph;
   mat4_orthagonal(VIEWPORT_WIDTH, VIEWPORT_HEIGHT, orthograph);
   glUniformMatrix4fv(uniform_ortho, 1, GL_FALSE, orthograph);

   g_timeout_add(20, on_idle, (void*)area);
}