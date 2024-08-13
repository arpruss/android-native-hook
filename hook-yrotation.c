#include <sensor.h>
#include <dlfcn.h>
#include <math.h>
#include <android/log.h>

/*
 * Replace sensor rotation vector by one that only takes into account y-axis tilt.
 */

typedef int (*ASensorEventQueue_getEvents_t)(ASensorEventQueue* queue, ASensorEvent* events, size_t count);

ssize_t old_ASensorEventQueue_getEvents(ASensorEventQueue* queue, ASensorEvent* events, size_t count)
{
  return ((ASensorEventQueue_getEvents_t)dlsym(RTLD_NEXT, "ASensorEventQueue_getEvents"))(queue, events, count);
}

inline double yTilt(double x, double y, double z, double w) {
	return 2*(w*x+y*z);
}

ssize_t ASensorEventQueue_getEvents(ASensorEventQueue* queue, ASensorEvent* events, size_t count) {
	ssize_t n = old_ASensorEventQueue_getEvents(queue, events, count);
	for (ssize_t i = 0 ; i < n ; i++) {
		ASensorEvent* e = events+i;
		//__android_log_print(ANDROID_LOG_VERBOSE, "hook", "type %d", e->type);
		if (e->type == ASENSOR_TYPE_ROTATION_VECTOR || e->type == ASENSOR_TYPE_GAME_ROTATION_VECTOR) {
			double y = yTilt(e->data[0],e->data[1],e->data[2],e->data[3]);
			double angle = asin(y);
			e->data[0] = 0;
			e->data[1] = sin(angle/2);
			e->data[2] = cos(angle/2);
		}
		else if (e->type == ASENSOR_TYPE_GRAVITY || e->type == ASENSOR_TYPE_ACCELEROMETER) {
			e->data[0] = 0;
		}
	}
	return n;
}
