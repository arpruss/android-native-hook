#include <sensor.h>
#include <dlfcn.h>
#include <math.h>
#include <android/log.h>

double lastX,lastY,lastZ;

typedef int (*ASensorEventQueue_getEvents_t)(ASensorEventQueue* queue, ASensorEvent* events, size_t count);

ssize_t old_ASensorEventQueue_getEvents(ASensorEventQueue* queue, ASensorEvent* events, size_t count)
{
  return ((ASensorEventQueue_getEvents_t)dlsym(RTLD_NEXT, "ASensorEventQueue_getEvents"))(queue, events, count);
}

void qmult(float* out,float* q,float* r) {
            out[0] = r[3]*q[0]+r[0]*q[3]-r[1]*q[2]+r[2]*q[1];
            out[1] = r[3]*q[1]+r[0]*q[2]+r[1]*q[3]-r[2]*q[0];
            out[2] = r[3]*q[2]-r[0]*q[1]+r[1]*q[0]+r[2]*q[3];
	        out[3] = r[3]*q[3]-r[0]*q[0]-r[1]*q[1]-r[2]*q[2];
}

// [x,y,z,w]*[0,0,1,0]*[-x,-y,-z,w] = [x,y,z,w] * [-y,-z,-x]

double gravityAngle = 0;

ssize_t ASensorEventQueue_getEvents(ASensorEventQueue* queue, ASensorEvent* events, size_t count) {
	ssize_t n = old_ASensorEventQueue_getEvents(queue, events, count);
	for (ssize_t i = 0 ; i < n ; i++) {
		ASensorEvent* e = events+i;
			__android_log_print(ANDROID_LOG_VERBOSE, "hook", "Type = %d", e->type);
		if (e->type == ASENSOR_TYPE_GRAVITY) {
			if (e->data[2] != 0 || e->data[1] !=0) 
				gravityAngle = atan2(e->data[1],e->data[2]);
		}
		else if (e->type == ASENSOR_TYPE_ROTATION_VECTOR) {
			//double yzAngle = atan2(e->vector.y,e->vector.z);
			
			//double magnitude = sqrt(e->vector.x*e->vector.x+e->vector.y*e->vector.y+e->vector.z*e->vector.z);
			//double halfAngle = atan2(e->data[3],magnitude);
			//__android_log_print(ANDROID_LOG_VERBOSE, "hook", "q %lg %lg %lg ang %lg grav %lg yz %lg", e->data[0],e->data[1],e->data[2],halfAngle,gravityAngle,yzAngle);
			e->vector.y = e->data[1] = sin(gravityAngle/2);
			e->vector.x = e->data[0] = 0;
			e->vector.z = e->data[2] = cos(gravityAngle/2);
			e->data[3] = 0;

//			double x = 2 * (e->data[3]*e->data[1]+e->data[0]*e->data[2]);
//			double y = 2 * (-e->data[3]*e->data[0]+e->data[1]*e->data[2]);
//			double z = e->data[3]*e->data[3]-e->data[0]*e->data[0]-e->data[1]*e->data[1]+e->data[2]*e->data[2];
//			__android_log_print(ANDROID_LOG_VERBOSE, "hook", "[0,0,1] to %lg,%lg,%lg %lg %lg", x,y,z,atan2(y,z),gravityAngle);
			
			
//			yzAngle = atan2(e->vector.z,e->vector.x);
//			__android_log_print(ANDROID_LOG_VERBOSE, "hook1", "quat=%lg %lg %lg %lg, xz angle %lg", e->vector.x,e->vector.y,e->vector.z, e->data[3], yzAngle);
			//e->data[0] = 0;
			//e->data[1] = 0;
		}
	}
	return n;
}
