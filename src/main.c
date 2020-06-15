#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#include <stdbool.h>
#include <assert.h>
#include "util/log.h"
#include "util/clock.h"
#include "ship.h"
#include "ship_cam.h"

#define SCREEN_WIDTH_PX 1000
#define SCREEN_HEIGHT_PX 1000

#define MAX_UPDATES_PER_FRAME 5
#define UPDATE_DELTA_S 0.0166666

static SDL_GLContext glcontext;
static SDL_Window *window;

/**** CUBE MODEL *************************************************************/

static GLfloat cube_vertices[] = {
  -1.0f, -1.0f,  1.0f,  
   1.0f, -1.0f,  1.0f,
   1.0f,  1.0f,  1.0f,
  -1.0f,  1.0f,  1.0f,
  -1.0f, -1.0f, -1.0f,  
   1.0f, -1.0f, -1.0f,
   1.0f,  1.0f, -1.0f,
  -1.0f,  1.0f, -1.0f
}; /* size = 12 */

static GLfloat cube_colors[] = {
  1.0f, 0.f, 0.f,
  1.0f, 0.f, 0.f,
  1.0f, 0.f, 0.f,
  1.0f, 0.f, 0.f,
  0.0f, 1.f, 0.f,
  0.0f, 1.f, 0.f,
  0.0f, 1.f, 0.f,
  0.0f, 1.f, 0.f
}; /* size = 12 */

static GLubyte cube_indices[] = {
  0, 1, 2,
  0, 2, 3,
  1, 5, 6,
  1, 6, 2,
  5, 4, 7,
  5, 7, 6, 
  4, 0, 3,
  4, 3, 7,
  3, 2, 6,
  3, 6, 7,
  1, 0, 4, 
  1, 4, 5
}; /* size == 36 */

/**** SHIP MODEL *************************************************************/

static GLfloat ship_vertices[] = {
  -0.5f,  0.0f, -1.0f,
  -1.0f,  0.0f,  1.0f,
   1.0f,  0.0f, -1.0f,
   0.5f,  0.0f, -1.0f,
  -0.75f, 0.3f,  0.5f,
   0.75f, 0.3f,  0.5f,
   0.75f,-0.3f,  0.5f,
  -0.75f,-0.3f,  0.5f
}; /* size = 8 */

static GLfloat ship_colors[] = {
  1.0f, 1.0f, 1.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 1.0f, 1.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f,
  1.0f, 0.0f, 0.0f
}; /* size = 8 */

static GLubyte ship_indices[] = {
  0, 1, 4,
  4, 1, 5,
  1, 2, 5,
  2, 3, 5,
  0, 4, 3,
  4, 5, 3,
  0, 1, 7,
  1, 6, 7,
  1, 2, 6,
  0, 7, 3,
  3, 7, 6,
  2, 3, 6
}; /* size = 36 */

/*****************************************************************************/

static const int xz_grid_lines = 101; /* num lines per dimension; square grid */
static const int xz_grid_vertex_count = (xz_grid_lines * 2 * 2);
static const int xz_grid_vertex_component_count = xz_grid_vertex_count * 3;
static GLfloat xzgrid[1212]; 

/* generate_xz_grid - generates a vertex array which can be used to render
 * a grid in the world space x-z plane. The grid is centered about it's origin.
 *
 * @vertex_array - buffer to store generated vertices.
 * @ox,oy,oz - position vector to origin of the grid.
 * @d - distance between grid lines (unit: meters).
 * @nx - num lines perpendicular to x-axis in grid.
 * @nz - num lines perpendicular to z-axis in grid.
 *
 * note - the @vertex_array MUST be large enough to store all the generated
 * vertices or a seg fault will occur. The number of generated vertices can
 * be calculated as:
 *    
 *    num_vertices = (nx + ny) * 2 * 3
 *
 * why? nx + ny = num lines in grid, then have 2 points per line and 3 vertices
 * per point.
 *
 * note - nx and nz should be odd values to allow the grid to be evenly spaced
 * about the origin; the odd, extra line in each dimension is the center line
 * in that dimension. This approach is taken to allow the center line to be
 * rendered as the axes of the grid and have the grid evenly spaced about the
 * axes. If nx or nz is even, they are made odd by subtracting 1.
 */
void
generate_xz_grid(GLfloat *vertex_array, 
                 float ox, 
                 float oy, 
                 float oz, 
                 float d_m, 
                 int nx, 
                 int nz)
{
  assert(vertex_array != NULL);

  int nhx, nhz, v = 0;

  if(nx % 2 == 0)
    nx--;
  if(nz % 2 == 0)
    nz--;

  nhx = (nx - 1) / 2;
  nhz = (nz - 1) / 2;

  /* generate the lines perpendicular to the x-axis */
  for(int i = -nhx; i < nhx; ++i)
  {
    vertex_array[v++] = ox + (d_m * i);
    vertex_array[v++] = oy + 0.f;
    vertex_array[v++] = oz + (-nhz);

    vertex_array[v++] = ox + (d_m * i);
    vertex_array[v++] = oy + 0.f;
    vertex_array[v++] = oz + nhz;
  }

  /* generate the lines perpendicular to the z-axis */
  for(int i = -nhz; i < nhz; ++i)
  {
    vertex_array[v++] = ox + (-nhx);
    vertex_array[v++] = oy + 0.f;
    vertex_array[v++] = oz + d_m * i;

    vertex_array[v++] = ox + nhx;
    vertex_array[v++] = oy + 0.f;
    vertex_array[v++] = oz + (d_m * i);
  }
}

static void
init()
{
  log_init();

  GLenum glerror;

  if(SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    fprintf(stderr, "fatal: failed to init SDL2: SDL error: %s\n", SDL_GetError());
    exit(EXIT_SUCCESS);
  }

  window = SDL_CreateWindow("sdl2-opengl", 
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         SCREEN_WIDTH_PX,
                         SCREEN_HEIGHT_PX,
                         SDL_WINDOW_OPENGL);
  if(!window)
  {
    fprintf(stderr, "fatal: failed to create window: SDL error: %s\n", SDL_GetError());
    exit(EXIT_SUCCESS);
  }

  glcontext = SDL_GL_CreateContext(window);
  if(!glcontext)
  {
    fprintf(stderr, "fatal: failed to create opengl context: SDL error: %s\n", SDL_GetError());
    exit(EXIT_SUCCESS);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  
  assert((!SDL_GL_SetSwapInterval(1) || !SDL_GL_SetSwapInterval(0)));

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if((glerror = glGetError()) != GL_NO_ERROR)
  {
    fprintf(stderr, "fatal: glMatrixMode: opengl error: %s\n", gluErrorString(glerror));
    exit(EXIT_SUCCESS);
  }

  gluPerspective(60.0f, 
                 SCREEN_WIDTH_PX / SCREEN_HEIGHT_PX,
                 1.0f,
                 1024.0f);

  glClearColor(0.f, 0.f, 0.f, 1.f);
  if((glerror = glGetError()) != GL_NO_ERROR)
  {
    fprintf(stderr, "fatal: glClearColor: opengl error: %s\n", gluErrorString(glerror));
    exit(EXIT_SUCCESS);
  }

  glViewport(0, 0, (GLsizei)SCREEN_WIDTH_PX, (GLsizei)SCREEN_HEIGHT_PX);
}

static void
run()
{
  struct clock real_clock;
  clock_init(&real_clock, CLOCK_MONOTONIC);
  log_write(LOG_INFO, 
            "real-time clock resolution = %ld nanoseconds\n", 
            clock_resolution_ns(&real_clock));


  generate_xz_grid(xzgrid, 0.f, 0.f, 0.f, 1.f, xz_grid_lines, xz_grid_lines);

  glEnableClientState(GL_VERTEX_ARRAY);
      
  glCullFace(GL_BACK);
  glFrontFace(GL_CCW);
  glEnable(GL_CULL_FACE);

  struct ship nautilus;
  memset((void *)&nautilus, 0, sizeof(struct ship));

  struct ship_cam cam;
  cam.target = &nautilus;
  cam.seperation_m = 20.f;

  float angle_deg = 0.f;
  float angle_vel_degPs = 10.f;

  double next_update_s = UPDATE_DELTA_S;
  bool redraw = true;
  bool is_done = false;
  int update_count;
  clock_reset(&real_clock);
  while(!is_done)
  {
    /* handle window events */
    SDL_Event event;
    while(SDL_PollEvent(&event))
    {
      switch(event.type)
      {
      case SDL_QUIT:
        is_done = true;
      case SDL_KEYDOWN:
        if(event.key.repeat != 0)
        {
          break;
        }
        if(event.key.keysym.sym == SDLK_w)
        {
          nautilus.is_boosting = true;
        }
        else if(event.key.keysym.sym == SDLK_UP)
        {
          ship_start_pitch(&nautilus, ROTATE_CW);
        }
        else if(event.key.keysym.sym == SDLK_DOWN)
        {
          ship_start_pitch(&nautilus, ROTATE_CCW);
        }
        else if(event.key.keysym.sym == SDLK_LEFT)
        {
          ship_start_roll(&nautilus, ROTATE_CCW);
        }
        else if(event.key.keysym.sym == SDLK_RIGHT)
        {
          ship_start_roll(&nautilus, ROTATE_CW);
        }
        break;
      case SDL_KEYUP:
        if(event.key.repeat != 0)
        {
          break;
        }
        if(event.key.keysym.sym == SDLK_w)
        {
          nautilus.is_boosting = false;
        }
        else if(event.key.keysym.sym == SDLK_UP)
        {
          ship_stop_pitch(&nautilus, ROTATE_CW);
        }
        else if(event.key.keysym.sym == SDLK_DOWN)
        {
          ship_stop_pitch(&nautilus, ROTATE_CCW);
        }
        else if(event.key.keysym.sym == SDLK_LEFT)
        {
          ship_stop_roll(&nautilus, ROTATE_CCW);
        }
        else if(event.key.keysym.sym == SDLK_RIGHT)
        {
          ship_stop_roll(&nautilus, ROTATE_CW);
        }
        break;
      }
    }

    double time_s = clock_time_s(&real_clock);
    update_count = 0;
    while(time_s > next_update_s && update_count < MAX_UPDATES_PER_FRAME)
    {
      angle_deg += angle_vel_degPs * UPDATE_DELTA_S;

      ship_update(&nautilus, UPDATE_DELTA_S);
      ship_cam_update(&cam, UPDATE_DELTA_S);

      next_update_s += UPDATE_DELTA_S;
      ++update_count;
      redraw = true;
    }

    if(redraw)
    {
      glClear(GL_COLOR_BUFFER_BIT);

      /* set camera position - i.e. view matrix */
      //glMatrixMode(GL_MODELVIEW);
      //glLoadIdentity();
      //glTranslatef(0.0f, 5.0f, -20.0f);
      //glRotatef(30.f, 1.0f, 0.0f, 0.0f);
      //glRotatef(angle_deg, 1.0f, 0.0f, 0.0f);
      
      glMatrixMode(GL_MODELVIEW);
      glLoadMatrixf(flatten44fm(&cam.view)); 

      glDisableClientState(GL_COLOR_ARRAY);

      /* bottom grid */
      glVertexPointer(3, GL_FLOAT, 0, xzgrid);
      glColor3f(1.f, 0.f, 0.f);
      glPushMatrix();
      glTranslatef(0.0f, -50.f, 0.f);
      glDrawArrays(GL_LINES, 0, xz_grid_vertex_count);
      glPopMatrix();

      /* top grid */
      glPushMatrix();
      glTranslatef(0.0f, 50.f, 0.f);
      glDrawArrays(GL_LINES, 0, xz_grid_vertex_count);
      glPopMatrix();

      /* back grid (+)z */
      glPushMatrix();
      glTranslatef(0.f, 0.f, 50.f);
      glRotatef(90.f, 1.0f, 0.0f, 0.0f);
      glDrawArrays(GL_LINES, 0, xz_grid_vertex_count);
      glPopMatrix();

      /* front grid (-)z */
      glPushMatrix();
      glTranslatef(0.f, 0.f, -50.f);
      glRotatef(90.f, 1.0f, 0.0f, 0.0f);
      glDrawArrays(GL_LINES, 0, xz_grid_vertex_count);
      glPopMatrix();

      glEnableClientState(GL_COLOR_ARRAY);

      /* draw cube */
      glPushMatrix();
      glRotatef(angle_deg, 0.0f, 1.0f, 0.0f);
      glVertexPointer(3, GL_FLOAT, 0, cube_vertices);
      glColorPointer(3, GL_FLOAT, 0, cube_colors);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cube_indices);
      glPopMatrix();

      /* draw ship */
      glPushMatrix();
      glLoadMatrixf(flatten44fm(&nautilus.mw)); 
      glEnableClientState(GL_COLOR_ARRAY);
      glVertexPointer(3, GL_FLOAT, 0, ship_vertices);
      glColorPointer(3, GL_FLOAT, 0, ship_colors);
      glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, ship_indices);
      glPopMatrix();

      SDL_GL_SwapWindow(window);
      redraw = false;
    }
  }
}

static void
shutdown()
{
}

int 
main(int argc, char *argv[])
{
  init();
  run();
  shutdown();
  exit(EXIT_SUCCESS);
}


