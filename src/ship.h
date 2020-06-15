#ifndef _SHIP_H_
#define _SHIP_H_

#include <stdbool.h>
#include "math/vector4f.h"
#include "math/matrix44f.h"

enum rotation
{
  ROTATE_CCW = 1,
  ROTATE_CW = -1
};

struct ship
{
  struct matrix44f mw;
  struct vector4f pos_W_m; 
  struct vector4f vel_W_mPs;
  float pitch_deg;
  float roll_deg;
  float pitch_vel_degPs;
  float roll_vel_degPs;
  float pitch_acc_degPs2;
  float roll_acc_degPs2;
  bool is_boosting;
};

void
ship_update(struct ship *sh, float delta_s);

void
ship_start_roll(struct ship *sh, enum rotation rot);

void
ship_stop_roll(struct ship *sh, enum rotation rot);

void
ship_start_pitch(struct ship *sh, enum rotation rot);

void
ship_stop_pitch(struct ship *sh, enum rotation rot);

struct matrix44f *
ship_get_world(struct ship *sh);

#endif
