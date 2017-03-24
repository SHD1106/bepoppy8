/*
 *
 * This file is part of paparazzi
 *
 */

/*
 * @file "modules/bepoppy8/bepoppy8.h"
 * Object Avoidance in the TU Delft CyberZoo using the onboard sensors of the Parrot Bebop Drone
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "bepoppy8.h"
#include "modules/computer_vision/lib/vision/image.h"

extern struct image_t *vision_func(struct image_t *);

#ifdef __cplusplus
}
#endif

#if DEBUGGING
#define logTelemetry(msg)	bepoppy8_logTelemetry(msg, (int) strlen(msg));
#else
#define logTelemetry(...)
#endif

struct ArrayInfo{
	uint8_t InitPoint = 0;
	uint8_t ArrayLength = 0;
};

int rowScans = 5;

struct ClusterInfo{
	uint8_t Cl0Global = 0;
	uint8_t Cl1Global = 0;
	uint8_t Cl2Global = 0;
};

struct ClusterInfo Environment;

uchar FloorID = 0;
int NavWindow = 0;




















