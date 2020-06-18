
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "config.h"
#include "util/system.h"
#include "math/mathutil.h"
#include "ship.h"
#include "ship_cam.h"


/* stores a new target MW in the history and returns the new oldest record;
   shallow copies the new_MW into the history */
struct matrix44f *
record_target_MW(struct ship_cam * cam, struct matrix44f *new_MW)
{
  assert(cam != NULL);
  assert(new_MW != NULL);

  struct matrix44f *temp;

  /* temp pointer to the last record; is about to be shifted off the end */
  temp = cam->target_MW_history[SHIP_MW_HISTORY_SIZE - 1];

  /* shift all records, freeing up element 0 for the new MW */
  for(int i = SHIP_MW_HISTORY_SIZE - 1; i >= 1; --i)
    cam->target_MW_history[i] = cam->target_MW_history[i - 1]; 

  memcpy((void *)temp, (void *)new_MW, sizeof(struct matrix44f));
  cam->target_MW_history[0] = temp;

  if(cam->oldest_record < SHIP_MW_HISTORY_SIZE - 1)
    cam->oldest_record++;

  return cam->target_MW_history[cam->oldest_record];
}

void
ship_cam_init(struct ship_cam *cam,
              struct ship *target,
              float seperation_m)
{
  cam->target = target;
  cam->seperation_m = seperation_m;
  cam->oldest_record = -1;
  for(int i = 0; i < SHIP_MW_HISTORY_SIZE; ++i)
    cam->target_MW_history[i] = xmalloc(sizeof(struct matrix44f));
}

void
ship_cam_free(struct ship_cam *cam)
{
  for(int i = 0; i < SHIP_MW_HISTORY_SIZE; ++i)
  {
    free(cam->target_MW_history[i]);
    cam->target_MW_history[i] = NULL;
  }
}

/* should be updated after the target ship */
void
ship_cam_tick(struct ship_cam *cam)
{
  struct vector4f eye_W_m, I, J, K;
  struct matrix44f *oldest_MW;

  oldest_MW = record_target_MW(cam, &(cam->target->MW));

  /* compute the eye position w.r.t world space */
  eye_W_m = multiply44fm(oldest_MW, 
                         (struct vector4f){0.f, 0.f, cam->seperation_m, 1.f});

  /* compute the view space unit basis vectors w.r.t world space */
  I = multiply44fm(oldest_MW, (struct vector4f){1.f, 0.f, 0.f, 0.f});
  J = multiply44fm(oldest_MW, (struct vector4f){0.f, 1.f, 0.f, 0.f});
  K = multiply44fm(oldest_MW, (struct vector4f){0.f, 0.f, 1.f, 0.f});

  /* construct the world-to-view matrix */
  worldview44fm(I, J, K, eye_W_m, &(cam->view));
}
