//const float ship_min_speed_mPs = 0.f;
//const float ship_max_speed_mPs = 20.f;
//const float ship_acc_mPs2 = 5.f;
//const float ship_max_cam_pitch_deg = 30.f;
//const float ship_max_cam_roll_deg = 30.f;
//const float ship_max_pitch_vel_degPs = 60.f;
//const float ship_max_roll_vel_degPs = 60.f;
//const float ship_pitch_acc_degPs2 = 10.f;
//const float ship_roll_acc_degPs2 = 10.f;

#include <string.h>
#include <assert.h>

#include "config.h"
#include "math/matrix44f.h"
#include "math/vector4f.h"

#include "ship.h"

static float delta_pitch_deg = SHIP_PITCH_VELOCITY_DEG_PER_S * TICK_DELTA_S;
static float delta_roll_deg = SHIP_ROLL_VELOCITY_DEG_PER_S * TICK_DELTA_S;

static inline void
recalc_roll_matrices(struct ship *sh)
{
  /* note - (+) angle is CCW */

  rotation_n44fm(delta_roll_deg,
                 sh->front.x,
                 sh->front.y,
                 sh->front.z,
                 &(sh->Rr[0]));

  memcpy((void *)&(sh->Rr[1]), (void *)&(sh->Rr[0]), sizeof(struct matrix44f));
  transpose44fm(&(sh->Rr[1]));
}

static inline void
recalc_pitch_matrices(struct ship *sh)
{
  /* note - (+) angle is CCW */

  rotation_n44fm(delta_pitch_deg, 
                 sh->right.x, 
                 sh->right.y, 
                 sh->right.z, 
                 &(sh->Rp[0]));

  memcpy((void *)&(sh->Rp[1]), (void *)&(sh->Rp[0]), sizeof(struct matrix44f));
  transpose44fm(&(sh->Rp[1]));
}

static inline void
recalc_model_world(struct ship *sh)
{
  struct vector4f J, K;

  K = scale4fv(sh->front, -1.f);
  J = cross4fv(K, sh->right);

  modelworld44fm(sh->right, J, K, sh->pos_W_m, &(sh->MW));
}

void
ship_init(struct ship *sh,
          struct vector4f pos_W_m,
          struct vector4f at_W_m,
          struct vector4f up_W_m)
{
  assert(pos_W_m.w == 1.f);
  assert(at_W_m.w == 1.f);
  assert(up_W_m.w == 0.f);

  memset((void *)sh, 0, sizeof(struct ship));

  sh->pos_W_m = pos_W_m;

  sh->front = sub4fv(pos_W_m, at_W_m);
  sh->front = normalise4fv(sh->front);

  sh->right = cross4fv(sh->front, up_W_m);
  sh->right = normalise4fv(sh->right);

  recalc_roll_matrices(sh);
  recalc_pitch_matrices(sh);

  recalc_model_world(sh);
}

void
ship_tick(struct ship *sh)
{
  
  static float ds = SHIP_BOOST_ACCELERATION_M_PER_S2 * TICK_DELTA_S;

  if(sh->rolling)
  {
    /* rotate 'right' direction by roll angle about the roll vector */
    sh->right = multiply44fm(&(sh->Rr[sh->rolling - 1]), sh->right);

    /* recalculate the pitch matrices as the pitch direction has changed */
    recalc_pitch_matrices(sh);
  }

  if(sh->pitching)
  {
    /* rotate 'front' direction by the pitch angle about the pitch vector */
    sh->front = multiply44fm(&(sh->Rp[sh->pitching - 1]), sh->front);

    /* recalculate the roll matrices as the roll vector has changed */
    recalc_roll_matrices(sh);
  }

  if(sh->boosting)
  {
    sh->speed_mPs += ds * sh->boosting;

    if(sh->speed_mPs > SHIP_MAX_SPEED_M_PER_S)
      sh->speed_mPs = SHIP_MAX_SPEED_M_PER_S;

    else if(sh->speed_mPs < SHIP_MIN_SPEED_M_PER_S)
      sh->speed_mPs = SHIP_MIN_SPEED_M_PER_S;
  }

  /* integrate the ship position */
  sh->pos_W_m = add4fv(sh->pos_W_m, scale4fv(sh->front, sh->speed_mPs));

  recalc_model_world(sh);

  ///* integrate orientation */
  //sh->pitch_vel_degPs += sh->pitch_acc_degPs2 * delta_s;
  //sh->roll_vel_degPs += sh->roll_acc_degPs2 * delta_s;
  //sh->pitch_deg += sh->pitch_vel_degPs * delta_s;
  //sh->roll_deg += sh->roll_vel_degPs * delta_s;

  ///* compute rotation component of the world matrix */
  //struct matrix44f r, p;
  //rotation_x44fm(sh->pitch_deg, &p);
  //rotation_z44fm(sh->roll_deg, &r);
  //concatenate44fm(&r, &p, &(sh->mw));

  ///* if boosting integrate linear velocity */
  //if(sh->is_boosting)
  //{
  //  struct vector4f dv = {0.f, 0.f, -1.f, 0.f}; /* dv = delta velocity */
  //  dv = multiply44fm(&(sh->mw), dv);
  //  dv = scale4fv(dv, ship_acc_mPs2 * delta_s);
  //  sh->vel_W_mPs = add4fv(sh->vel_W_mPs, dv);
  //}

  ///* integrate the ship pos */
  //sh->pos_W_m = add4fv(sh->pos_W_m, scale4fv(sh->vel_W_mPs, delta_s));

  ///* add the translation to the world matrix */
  //sh->mw.m[3][0] = sh->pos_W_m.x;
  //sh->mw.m[3][1] = sh->pos_W_m.y;
  //sh->mw.m[3][2] = sh->pos_W_m.z;
  
}

