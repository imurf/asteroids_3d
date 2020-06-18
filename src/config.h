#ifndef _CONFIG_H_
#define _CONFIG_H_

/* The time interval each game tick integrates over (unit: seconds). */
#define TICK_DELTA_S 0.0166666

/* the upper limit on the number of game ticks performed in a single game loop 
   iteration */
#define MAX_TICKS_PER_FRAME 5

/*** SHIP CONFIG *************************************************************/

#define SHIP_ROLL_VELOCITY_DEG_PER_S  60.0
#define SHIP_PITCH_VELOCITY_DEG_PER_S 60.0

#define SHIP_BOOST_ACCELERATION_M_PER_S2 2.0

#define SHIP_MIN_SPEED_M_PER_S 0.0
#define SHIP_MAX_SPEED_M_PER_S 10.0

/*** FOLLOW CAMERA CONFIG *****************************************************/

/* the delay between the follow camera and the ship; the camera is actually
   following the ship as it was FOLLOW_DELAY_S ago. This creates the lagging
   effect on the camera that makes visible the ships movement. The larger the
   delay the 'stronger' the lagging effect. */
#define FOLLOW_DELAY_S 0.5

/* the number of the ship's past model-world matrices the camera stores. This
   value must be precomputed and is given by:
        = (int)(FOLLOW_DELAY_S / TICK_DELTA_S) */
#define SHIP_MW_HISTORY_SIZE 30

#endif
