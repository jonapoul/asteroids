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
   player.left        = FALSE;
   player.right       = FALSE;
   player.angle       = 0.0;
   player.x_speed     = 0.0;
   player.y_speed     = 0.0;
   player.force       = 0.1;
   player.max_speed   = 8.0;
   player.angle_speed = 0.05;
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