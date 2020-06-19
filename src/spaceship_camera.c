
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "config.h"
#include "util/system.h"
#include "math/mathutil.h"
#include "spaceship.h"
#include "spaceship_camera.h"

/* stores a new target MW in the history and returns the new oldest record;
   shallow copies the new_MW into the history */
static struct matrix44f *
record_mw(struct spaceship_camera *cam, struct matrix44f *mw)
{
  struct matrix44f *temp;

  /* temp pointer to the last record; is about to be shifted off the end */
  temp = cam->mw_history[SHIP_MW_HISTORY_SIZE - 1];

  /* shift all records, freeing up element 0 for the new MW */
  for(int i = SHIP_MW_HISTORY_SIZE - 1; i >= 1; --i)
    cam->mw_history[i] = cam->mw_history[i - 1]; 

  memcpy((void *)temp, (void *)mw, sizeof(struct matrix44f));
  cam->mw_history[0] = temp;

  if(cam->oldest_mw < SHIP_MW_HISTORY_SIZE - 1)
    cam->oldest_mw++;

  return cam->mw_history[cam->oldest_mw];
}

void
shipcam_init(struct spaceship_camera *cam, struct spaceship *target)
{
  cam->target = target;
  cam->oldest_mw = -1;

  for(int i = 0; i < SHIP_MW_HISTORY_SIZE; ++i)
    cam->mw_history[i] = xmalloc(sizeof(struct matrix44f));
}

void
shipcam_free(struct spaceship_camera *cam)
{
  for(int i = 0; i < SHIP_MW_HISTORY_SIZE; ++i)
  {
    free(cam->mw_history[i]);
    cam->mw_history[i] = NULL;
  }
}

/* should be updated after the target ship */
void
shipcam_tick(struct spaceship_camera *cam)
{
  struct vector4f eye_w_m, I, J, K;
  struct matrix44f *mw;

  mw = record_mw(cam, &(cam->target->mw));

  /* compute the eye position w.r.t world space */
  eye_w_m = multiply44fm(mw, 
                         (struct vector4f){0.f, 
                                           SHIPCAM_VIEW_HEIGHT_M, 
                                           SHIPCAM_MIN_VIEW_DISTANCE_M, 
                                           1.f});

  /* compute the view space unit basis vectors w.r.t world space */
  I = multiply44fm(mw, (struct vector4f){1.f, 0.f, 0.f, 0.f});
  J = multiply44fm(mw, (struct vector4f){0.f, 1.f, 0.f, 0.f});
  K = multiply44fm(mw, (struct vector4f){0.f, 0.f, 1.f, 0.f});

  /* construct the world-to-view matrix */
  worldview44fm(I, J, K, eye_w_m, &(cam->wv));
}
