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

#endif
