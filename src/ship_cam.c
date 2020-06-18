
#include "math/mathutil.h"
#include "ship.h"
#include "ship_cam.h"

#include <stdio.h>
#include <math.h>

/* should be updated after the ship */
void
ship_cam_tick(struct ship_cam *cam)
{
  struct vector4f eye_W_m, I, J, K;

  /* compute the eye position w.r.t world space */
  eye_W_m = multiply44fm(&(cam->target->MW), 
                         (struct vector4f){0.f, 0.f, cam->seperation_m, 1.f});

  /* compute the view space unit basis vectors w.r.t world space */
  I = multiply44fm(&(cam->target->MW), (struct vector4f){1.f, 0.f, 0.f, 0.f});
  J = multiply44fm(&(cam->target->MW), (struct vector4f){0.f, 1.f, 0.f, 0.f});
  K = multiply44fm(&(cam->target->MW), (struct vector4f){0.f, 0.f, 1.f, 0.f});

  /* construct the world-to-view matrix */
  worldview44fm(I, J, K, eye_W_m, &(cam->view));
}
