#ifndef _SHIP_CAM_H_
#define _SHIP_CAM_H_

#include "config.h"

struct ship_cam
{
  struct ship *target; /* ship being followed */
  struct matrix44f view; /* view matrix of the camera */
  float seperation_m; /* distance between eye and ship center */

  /* history of model-world matrices (MW) of the target for the last 
     FOLLW_DELAY_S seconds, used to implement the movement effects. The
     most recent record is stored at array position 0, the oldest record at
     array position 'oldest_record' */
  int oldest_record; 
  struct matrix44f *target_MW_history[SHIP_MW_HISTORY_SIZE];
};

void
ship_cam_init(struct ship_cam *cam, 
              struct ship *target,
              float seperation_m);

/* should be ticked after the ship */
void
ship_cam_tick(struct ship_cam *cam);

#endif
