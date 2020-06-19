#ifndef _CONFIG_H_
#define _CONFIG_H_

/* The time interval each game tick integrates over (unit: seconds). */
#define TICK_DELTA_S 0.0166666

/* the upper limit on the number of game ticks performed in a single game loop 
   iteration */
#define MAX_TICKS_PER_FRAME 5

/*** SHIP CONFIG *************************************************************/

/* anglular velocity limits of ship: limits symmetrical for CW, CCW rotations */
#define SHIP_MAX_ROLL_DG_P_S 120.f
#define SHIP_MAX_PITCH_DG_P_S 120.f

/* scalar angular accelerations of ship (constant) */
#define SHIP_ROLL_DG_P_S2  240.f
#define SHIP_PITCH_DG_P_S2 240.f

/* ship scalar speed limits */
#define SHIP_MIN_POS_M_P_S 0.5f
#define SHIP_MAX_POS_M_P_S 1.0f

/* ship scalar linear acceleration (constant) */
#define SHIP_POS_M_P_S2 1.f

/*** FOLLOW CAMERA CONFIG *****************************************************/

/* the delay between the follow camera and the ship; the camera is actually
   following the ship as it was FOLLOW_DELAY_S ago. This creates the lagging
   effect on the camera that makes visible the ships movement. The larger the
   delay the 'stronger' the lagging effect. */
#define FOLLOW_DELAY_S 0.2

/* the number of the ship's past model-world matrices the camera stores. This
   value must be precomputed and is given by:
        = (int)(FOLLOW_DELAY_S / TICK_DELTA_S) */
#define SHIP_MW_HISTORY_SIZE 12 

/* controls how close the camera can get to the ship; distance when ship at rest */
#define SHIPCAM_MIN_VIEW_DISTANCE_M 5.f

/* height above the ship of the camera */
#define SHIPCAM_VIEW_HEIGHT_M 5.f

#endif
