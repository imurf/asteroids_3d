const float ship_min_speed_mPs = 0.f;
const float ship_max_speed_mPs = 20.f;
const float ship_acc_mPs2 = 5.f;
const float ship_max_cam_pitch_deg = 30.f;
const float ship_max_cam_roll_deg = 30.f;
const float ship_max_pitch_vel_degPs = 60.f;
const float ship_max_roll_vel_degPs = 60.f;
const float ship_pitch_acc_degPs2 = 10.f;
const float ship_roll_acc_degPs2 = 10.f;

#include "math/matrix44f.h"
#include "math/vector4f.h"

#include "ship.h"

void
ship_update(struct ship *sh, float delta_s)
{
  /* integrate orientation */
  sh->pitch_vel_degPs += sh->pitch_acc_degPs2 * delta_s;
  sh->roll_vel_degPs += sh->roll_acc_degPs2 * delta_s;
  sh->pitch_deg += sh->pitch_vel_degPs * delta_s;
  sh->roll_deg += sh->roll_vel_degPs * delta_s;

  /* compute rotation component of the world matrix */
  struct matrix44f r, p;
  rotation_x44fm(sh->pitch_deg, &p);
  rotation_z44fm(sh->roll_deg, &r);
  concatenate44fm(&r, &p, &(sh->mw));

  /* if boosting integrate linear velocity */
  if(sh->is_boosting)
  {
    struct vector4f dv = {0.f, 0.f, -1.f, 1.f}; /* dv = delta velocity */
    dv = multiply44fm(&(sh->mw), dv);
    dv = scale4fv(dv, ship_acc_mPs2 * delta_s);
    sh->vel_W_mPs = add4fv(sh->vel_W_mPs, dv);
  }

  /* integrate the ship pos */
  sh->pos_W_m = add4fv(sh->pos_W_m, scale4fv(sh->vel_W_mPs, delta_s));

  /* add the translation to the world matrix */
  sh->mw.m[3][0] = sh->pos_W_m.x;
  sh->mw.m[3][1] = sh->pos_W_m.y;
  sh->mw.m[3][2] = sh->pos_W_m.z;
}

void
ship_start_roll(struct ship *sh, enum rotation rot)
{
  sh->roll_acc_degPs2 = rot * ship_roll_acc_degPs2;
}

void
ship_stop_roll(struct ship *sh, enum rotation rot)
{
  sh->roll_acc_degPs2 = 0.f;
}

void
ship_start_pitch(struct ship *sh, enum rotation rot)
{
  sh->pitch_acc_degPs2 = rot * ship_pitch_acc_degPs2;
}

void
ship_stop_pitch(struct ship *sh, enum rotation rot)
{
  sh->pitch_acc_degPs2 = 0.f;
}

struct matrix44f *
ship_get_world(struct ship *sh)
{
  return &(sh->mw);
}

