
#ifndef _callbacks_h
#define _callbacks_h

void renderInit( void );
void renderRunning( void );
void renderGameover( void );
void registarCallbackFunctions( void );
int glhUnProjectf(float winx, float winy, float winz, mat4x4 *modelview, mat4x4 *projection, int *viewport, float *objectCoordinate);

#endif
