#ifndef _SHIP_H_
#define _SHIP_H_

#include <stdbool.h>
#include <inttypes.h>

#include "math/vector4f.h"
#include "math/matrix44f.h"

enum rotation {ROTATE_NONE = 0, ROTATE_CCW = 1, ROTATE_CW = 2};
enum boost {BOOST_REVERSE = -1, BOOST_NONE = 0, BOOST_FORWARD = 1};

struct spaceship
{
  /* position and derivatives w.r.t world space (unit: meters); scalar derivative 
     'pos_w_m_p_s' always in direction of 'front' vector */
  struct vector4f vpos_w_m; 
  float pos_w_m_p_s;

  /* model-world matrix */
  struct matrix44f mw;

  /* direction vectors to define orientation of ship:
      front = direction ship's nose is pointing
      right = perpendicular vector to front */
  struct vector4f front;
  struct vector4f right;

  /* rotation matrices used to perform roll/pitch transformations. There are
     2 matrices for each as matrix[0] does CCW rotation, matrix[1] does CW */
  struct matrix44f Rp[2];
  struct matrix44f Rr[2];

  /* angular speeds of pitch/roll rotations */
  float roll_dg_p_s;
  float pitch_dg_p_s;

  /* precomputed changes in angular position, i.e. speed * TICK_DELTA_S, recalculated 
     each time the angular speeds change */
  float delta_roll_dg;
  float delta_pitch_dg;

  /* movement flags */
  enum boost boosting;
  enum rotation rolling;
  enum rotation pitching;
};

/* ship_init - sets the ship to an initial valid state; must be called prior
 *   to use of a ship instance. Arguments control the initial orientation of
 *   the ship in world space.
 *
 * @pos_W_m - initial position vector to the ship origin (center of ship) in
 *   world space coordinates.
 * @at_W_m - position vector to a world space point the ship's nose is pointing
 *   at.
 * @up_W_m - a direction vector which defines the roll orientation of the ship,
 *   i.e. the 'up' direction of the ship w.r.t world space.
 */
void
spaceship_init(struct spaceship *sh,
               struct vector4f pos_W_m,
               struct vector4f at_W_m,
               struct vector4f up_W_m);

void
spaceship_tick(struct spaceship *sh);

static inline void
spaceship_roll(struct spaceship *sh, enum rotation r){sh->rolling = r;}

static inline void
spaceship_pitch(struct spaceship *sh, enum rotation r){sh->pitching = r;}

static inline void
spaceship_boost(struct spaceship *sh, enum boost b){sh->boosting = b;}

#endif
