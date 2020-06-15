
#include "math/mathutil.h"
#include "ship.h"
#include "ship_cam.h"

/* should be updated after the ship */
void
ship_cam_update(struct ship_cam *cam, float delta_s)
{
  /* compute the eye position w.r.t world space */
  struct vector4f eye_W_m = {0.f, 0.f, -(cam->seperation_m), 1.f};
  eye_W_m = multiply44fm(&(cam->target->mw), eye_W_m);

  /* compute the view matrix */
  view_look_at(eye_W_m, 
               cam->target->pos_W_m, 
               (struct vector4f){0.f, 1.f, 0.f, 0.f}, 
               &(cam->view)); 
}
