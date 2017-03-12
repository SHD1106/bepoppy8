/*
 *
 * This file is part of paparazzi
 *
 */

/*
 * @file "modules/bepoppy8/bepoppy8.c"
 * Object Avoidance in the TU Delft CyberZoo using the on-board sensors of the Parrot Bebop Drone
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "firmwares/rotorcraft/navigation.h"
#include "subsystems/datalink/telemetry.h"
#include "generated/flight_plan.h"
#include "modules/bepoppy8/bepoppy8.h"

#if DEBUGGING
#define logTelemetry(msg)	bepoppy8_logTelemetry(msg, (int) strlen(msg));
#else
#define logTelemetry(...)
#endif

void bepoppy8_init() {
	// Initial values to be defined at start-up of module
	logTelemetry("Bepoppy8_init");
}

void bepoppy8_periodic() {
	// Periodic function that processes the video and decides on the action to take.
	logTelemetry("Bepoppy8_periodic");
}


/*
 * Use to send debugging related strings to the terminal and GCS.
 *
 * #Note: 	Due to a bug in the Paparazzi Code, do NOT use delimiters as ) in the telemetry strings.
 *			There may be more delimiters that also cause errors.
 */
void bepoppy8_logTelemetry(char* msg, int nb_msg) {
	if (DEBUGGING){
		DOWNLINK_SEND_INFO_MSG(DefaultChannel, DefaultDevice, nb_msg,  msg);
		printf("%s", msg);
	}
}


/*
 * Move the current waypoint with the distances defined by *shift.
 */
void bepoppy8_moveWaypointBy(uint8_t waypoint, struct EnuCoor_i *shift){
	struct EnuCoor_i new_coor;

	struct EnuCoor_i *pos 	= stateGetPositionEnu_i();
	new_coor.x  			= pos->x + shift->x;
	new_coor.y 				= pos->y + shift->y;

	coordinateTurn(pos, shift, &nav_heading);

	if(shift->z != 0){
		new_coor.z 		= pos->z + shift->z;
		waypoint_set_enu_i (waypoint, &new_coor);
	}
	else{
		waypoint_set_xy_i(waypoint, new_coor.x, new_coor.y);
	}

}


/*
 * Move the current waypoint to the location defined by *new_coor.
 */
void bepoppy8_moveWaypointTo(uint8_t waypoint, struct EnuCoor_i *new_coor){
	struct EnuCoor_i *pos 	= stateGetPositionEnu_i();
	struct EnuCoor_i shift;

	shift.x 				= pos->x - new_coor->x;
	shift.y 				= pos->y - new_coor->y;
	shift.z 				= pos->z - new_coor->z;

	coordinateTurn(pos, &shift, &nav_heading);

	if(shift.z != 0){
		new_coor->z 		= pos->z + shift.z;
		waypoint_set_enu_i (waypoint, new_coor);
	}
	else{
		waypoint_set_xy_i(waypoint, new_coor->x, new_coor->y);
	}

}


/*
 * Move waypoint forward with an amount equal to the parameter distance.
 */
void bepoppy8_moveWaypointForward(uint8_t waypoint, float distance){
	struct EnuCoor_i shift;
	struct Int32Eulers *eulerAngles   	= stateGetNedToBodyEulers_i();

	// Calculate the sine and cosine of the heading the drone is keeping
	float sin_heading                 	= sinf(ANGLE_FLOAT_OF_BFP(eulerAngles->psi));
	float cos_heading                 	= cosf(ANGLE_FLOAT_OF_BFP(eulerAngles->psi));

	// Now determine where to place the waypoint you want to go to
	shift.x                       		= sin_heading * distance;
	shift.y                       		= cos_heading * distance;

	bepoppy8_moveWaypointBy(waypoint, &shift);
}


/*
 * Based on the shift vector, set the new heading of the rotorcraft.
 */
void coordinateTurn(struct EnuCoor_i *pos, struct EnuCoor_i *shift, int32_t *heading){

	int32_t newHeading 	= ANGLE_BFP_OF_REAL(atan2(shift->y,shift->x) + M_PI/2);
	INT32_ANGLE_NORMALIZE(newHeading);

	*heading 	= newHeading;
}


