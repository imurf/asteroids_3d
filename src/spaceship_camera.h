#ifndef _SHIP_CAM_H_
#define _SHIP_CAM_H_

#include "config.h"
#include "math/matrix44f.h"

struct spaceship;

struct spaceship_camera
{
  /* the spaceship being followed */
  struct spaceship *target; 

  /* the world-view matrix of the camera */
  struct matrix44f wv; 

  /* history of model-world matrices (MW) of the target for the last FOLLW_DELAY_S seconds, used 
     to implement the movement effects. The most recent mw is stored at array position 0, the 
     oldest at array position 'oldest_record' */
  int oldest_mw; 
  struct matrix44f *mw_history[SHIP_MW_HISTORY_SIZE];
};

void
shipcam_init(struct spaceship_camera *cam, struct spaceship *target);

/* should be ticked after the ship */
void
shipcam_tick(struct spaceship_camera *cam);

#endif
