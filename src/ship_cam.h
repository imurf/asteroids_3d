#ifndef _SHIP_CAM_H_
#define _SHIP_CAM_H_

struct ship_cam
{
  struct ship *target; /* ship being followed */
  struct matrix44f view; /* view matrix of the camera */
  float seperation_m; /* distance between eye and ship center */
};

/* should be ticked after the ship */
void
ship_cam_tick(struct ship_cam *cam);

#endif
