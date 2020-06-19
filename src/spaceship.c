
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "config.h"
#include "math/matrix44f.h"
#include "math/vector4f.h"

#include "spaceship.h"

/* precomputed changes in angular speed; both angular acceleration and the integration time
   are constant */
static float delta_roll_dg_p_s = SHIP_ROLL_DG_P_S2 * TICK_DELTA_S;
static float delta_pitch_dg_p_s = SHIP_PITCH_DG_P_S2 * TICK_DELTA_S;

/* precomputed changes in linear speed; linear acceleration constant */
static float delta_pos_w_m_p_s = SHIP_POS_M_P_S2 * TICK_DELTA_S;

static inline void
recalculate_roll_matrices(struct spaceship *sh, float angle_dg)
{
  /* note - (+) angle is CCW */

  if(angle_dg < 0.f)
    angle_dg *= -1.f;

  rotation_n44fm(angle_dg,
                 sh->front.x,
                 sh->front.y,
                 sh->front.z,
                 &(sh->Rr[0]));

  memcpy((void *)&(sh->Rr[1]), (void *)&(sh->Rr[0]), sizeof(struct matrix44f));
  transpose44fm(&(sh->Rr[1]));
}

static inline void
recalculate_pitch_matrices(struct spaceship *sh, float angle_dg)
{
  /* note - (+) angle is CCW */

  if(angle_dg < 0.f)
    angle_dg *= -1.f;

  rotation_n44fm(angle_dg, 
                 sh->right.x, 
                 sh->right.y, 
                 sh->right.z, 
                 &(sh->Rp[0]));

  memcpy((void *)&(sh->Rp[1]), (void *)&(sh->Rp[0]), sizeof(struct matrix44f));
  transpose44fm(&(sh->Rp[1]));
}

static inline void
recalculate_model_world(struct spaceship *sh)
{
  struct vector4f J, K;

  K = scale4fv(sh->front, -1.f);
  J = cross4fv(K, sh->right);

  modelworld44fm(sh->right, J, K, sh->vpos_w_m, &(sh->mw));
}

void
spaceship_init(struct spaceship *sh,
          struct vector4f pos_w_m,
          struct vector4f at_w_m,
          struct vector4f up_w_m)
{
  assert(pos_w_m.w == 1.f);
  assert(at_w_m.w == 1.f);
  assert(up_w_m.w == 0.f);

  memset((void *)sh, 0, sizeof(struct spaceship));

  sh->vpos_w_m = pos_w_m;
  sh->pos_w_m_p_s = SHIP_MIN_POS_M_P_S;

  sh->front = sub4fv(pos_w_m, at_w_m);
  sh->front = normalise4fv(sh->front);

  sh->right = cross4fv(sh->front, up_w_m);
  sh->right = normalise4fv(sh->right);

  recalculate_roll_matrices(sh, 0.f);
  recalculate_pitch_matrices(sh, 0.f);

  recalculate_model_world(sh);
}

void
spaceship_tick(struct spaceship *sh)
{
  bool roll_change = false, pitch_change = false;

  /* tick roll magnitude */

  switch(sh->rolling)
  {
  case ROTATE_NONE:
    
    /* 'decay' roll speed back down to 0 */

    if(sh->roll_dg_p_s > 0.f)
    {
      sh->roll_dg_p_s -= delta_roll_dg_p_s;
      if(sh->roll_dg_p_s < 0.f)
        sh->roll_dg_p_s = 0.f;
      roll_change = true;
    }
    else if(sh->roll_dg_p_s < 0.f)
    {
      sh->roll_dg_p_s += delta_roll_dg_p_s;
      if(sh->roll_dg_p_s > 0.f)
        sh->roll_dg_p_s = 0.f;
      roll_change = true;
    }
    break;

  case ROTATE_CCW:
    if(sh->roll_dg_p_s != SHIP_MAX_ROLL_DG_P_S)
    {
      sh->roll_dg_p_s += delta_roll_dg_p_s;
      if(sh->roll_dg_p_s > SHIP_MAX_ROLL_DG_P_S)
        sh->roll_dg_p_s = SHIP_MAX_ROLL_DG_P_S;
      roll_change = true;
    }
    break;

  case ROTATE_CW:
    if(sh->roll_dg_p_s != -SHIP_MAX_ROLL_DG_P_S)
    {
      sh->roll_dg_p_s -= delta_roll_dg_p_s;
      if(sh->roll_dg_p_s < -SHIP_MAX_ROLL_DG_P_S)
        sh->roll_dg_p_s = -SHIP_MAX_ROLL_DG_P_S;
      roll_change = true;
    }
  }

  /* if magnitude of roll has changed */

  if(roll_change)
  {
    sh->delta_roll_dg = sh->roll_dg_p_s * TICK_DELTA_S;
    recalculate_roll_matrices(sh, sh->delta_roll_dg);
  }

  /* perform roll */
  if(sh->roll_dg_p_s != 0.f)
  {
    /* rotate 'right' direction by the roll angle about the 'front' direction */
    sh->right = multiply44fm(&(sh->Rr[(sh->roll_dg_p_s > 0.f) ? 0 : 1]), sh->right);

    /* force recalculation of pitch matrices as the pitch direction, i.e. right, has changed */
    pitch_change = true;
  }

  switch(sh->pitching)
  {
  case ROTATE_NONE:
    
    /* 'decay' pitch speed back down to 0 */

    if(sh->pitch_dg_p_s > 0.f)
    {
      sh->pitch_dg_p_s -= delta_pitch_dg_p_s;
      if(sh->pitch_dg_p_s < 0.f)
        sh->pitch_dg_p_s = 0.f;
      pitch_change = true;
    }
    else if(sh->pitch_dg_p_s < 0.f)
    {
      sh->pitch_dg_p_s += delta_pitch_dg_p_s;
      if(sh->pitch_dg_p_s > 0.f)
        sh->pitch_dg_p_s = 0.f;
      pitch_change = true;
    }
    break;

  case ROTATE_CCW:
    if(sh->pitch_dg_p_s != SHIP_MAX_PITCH_DG_P_S)
    {
      sh->pitch_dg_p_s += delta_pitch_dg_p_s;
      if(sh->pitch_dg_p_s > SHIP_MAX_PITCH_DG_P_S)
        sh->pitch_dg_p_s = SHIP_MAX_PITCH_DG_P_S;
      pitch_change = true;
    }
    break;

  case ROTATE_CW:
    if(sh->pitch_dg_p_s != -SHIP_MAX_PITCH_DG_P_S)
    {
      sh->pitch_dg_p_s -= delta_pitch_dg_p_s;
      if(sh->pitch_dg_p_s < -SHIP_MAX_PITCH_DG_P_S)
        sh->pitch_dg_p_s = -SHIP_MAX_PITCH_DG_P_S;
      pitch_change = true;
    }
  }

  /* magnitude of pitch has changed */
  if(pitch_change)
  {
    sh->delta_pitch_dg = sh->pitch_dg_p_s * TICK_DELTA_S;
    recalculate_pitch_matrices(sh, sh->delta_pitch_dg);
  }

  if(sh->pitch_dg_p_s != 0.f)
  {
    /* rotate 'front' direction by the pitch angle about the 'right' direction */
    sh->front = multiply44fm(&(sh->Rp[(sh->pitch_dg_p_s > 0.f) ? 0 : 1]), sh->front);

    /* recalculate roll matrices as the roll direction, i.e. 'front', has changed */
    recalculate_roll_matrices(sh, sh->delta_roll_dg);
  }

  if((int)sh->boosting)
  {
    sh->pos_w_m_p_s += delta_pos_w_m_p_s * (int)sh->boosting;

    if(sh->pos_w_m_p_s > SHIP_MAX_POS_M_P_S)
      sh->pos_w_m_p_s = SHIP_MAX_POS_M_P_S;

    else if(sh->pos_w_m_p_s < SHIP_MIN_POS_M_P_S)
      sh->pos_w_m_p_s = SHIP_MIN_POS_M_P_S;
  }

  /* integrate the spaceship position */
  sh->vpos_w_m = add4fv(sh->vpos_w_m, scale4fv(sh->front, sh->pos_w_m_p_s));

  recalculate_model_world(sh);
}

