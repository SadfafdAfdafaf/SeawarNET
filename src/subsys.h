#ifndef __SUBSYS_H__
#define __SUBSYS_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "camera.h"
#include "keyboard.h"
#include "vector3.h"

#define WIN_WIDTH 640
#define WIN_HEIGHT 480

#define CAMERA_FOV 90
#define CAMERA_ASPECT (float)WIN_WIDTH / (float)WIN_HEIGHT
#define CAMERA_ZNEAR 1.0
#define CAMERA_ZFAR 1000.0

#define CAMERA_MOVE_SPEED 0.5
#define CAMERA_ROTATE_SPEED 0.07
#define CAMERA_ROLL_SPEED 0.01
#define N 400

void init_subgl(void);
void apply_camera_projection(camera_t *cam);
void apply_camera_view(camera_t *cam);
void apply_camera(camera_t *cam);
void process_keys(keystate_t keys, camera_t *cam);

#endif