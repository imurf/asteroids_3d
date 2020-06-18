#ifndef _SHIP_H_
#define _SHIP_H_

#include <stdbool.h>
#include <inttypes.h>

#include "math/vector4f.h"
#include "math/matrix44f.h"

enum rotation {ROTATE_NONE, ROTATE_CCW, ROTATE_CW};
enum boost {BOOST_REVERSE = -1, BOOST_NONE = 0, BOOST_FORWARD = 1};

struct ship
{
  /* position w.r.t world space (unit: meters) */
  struct vector4f pos_W_m; 

  /* model-world matrix */
  struct matrix44f MW;

  /* direction vectors to defined orientation of ship:
      front = direction ship's nose is pointing
      right = perpendicular vector to front */
  struct vector4f front;
  struct vector4f right;

  /* rotation matrices used to perform roll/pitch transformations. There are
     2 matrices for each as matrix[0] does CCW rotation, matrix[1] does CW */
  struct matrix44f Rp[2];
  struct matrix44f Rr[2];

  //struct vector4f vel_W_mPs;
  //float pitch_deg;
  //float roll_deg;
  //float pitch_vel_degPs;
  //float roll_vel_degPs;
  //float pitch_acc_degPs2;
  //float roll_acc_degPs2;
  
  /* current scalar speed of the ship in the direction of the 'front' vector */
  float speed_mPs;

  /* movement flags:
       boosting - (-1)=decelerating, 0=no boost, (+1)=accelerating
       pitching/rolling -  0=no rotation, 1=CCW, 2=CW */
  uint8_t boosting;
  uint8_t pitching;
  uint8_t rolling;
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
ship_init(struct ship *sh,
          struct vector4f pos_W_m,
          struct vector4f at_W_m,
          struct vector4f up_W_m);

void
ship_tick(struct ship *sh);

static inline void
ship_roll(struct ship *sh, enum rotation r){sh->rolling = r;}

static inline void
ship_pitch(struct ship *sh, enum rotation r){sh->pitching = r;}

static inline void
ship_boost(struct ship *sh, enum boost b){sh->boosting = b;}

#endif
