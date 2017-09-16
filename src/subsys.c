#include "subsys.h"

void init_subgl(void){
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void apply_camera_projection(camera_t *cam) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(cam->fov, cam->aspect, cam->znear, cam->zfar);
  //gluPerspective(cam->fov, cam->aspect, cam->zfar, cam->znear);
}

void apply_camera_view(camera_t *cam) {
  vec3_t center = vec3_addp(&cam->pos, &cam->fwd);
  
  glMatrixMode(GL_MODELVIEW);  
  glLoadIdentity();
  gluLookAt(cam->pos.x, cam->pos.y, cam->pos.z, 
            center.x, center.y, center.z,
            cam->up.x, cam->up.y, cam->up.z);  
            
  //printf("%f %f %f - %f %f %f\n", cam->pos.x, cam->pos.y, cam->pos.z, center.x, center.y, center.z);
}

void apply_camera(camera_t *cam) {
  apply_camera_projection(cam);
  apply_camera_view(cam);
}

void process_keys(keystate_t keys, camera_t *cam) {
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_SPACE))
    camera_move_up_y(cam, CAMERA_MOVE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_C))
    camera_move_down_y(cam, CAMERA_MOVE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_W))
    camera_move_fwd(cam, CAMERA_MOVE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_S))
    camera_move_back(cam, CAMERA_MOVE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_A))
    camera_move_left(cam, CAMERA_MOVE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_D))
    camera_move_right(cam, CAMERA_MOVE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_Q))
    camera_rotate_roll(cam, -CAMERA_ROLL_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_E))
    camera_rotate_roll(cam, CAMERA_ROLL_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_LEFT))
    camera_rotate_up(cam, CAMERA_ROTATE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_RIGHT))
    camera_rotate_up(cam, -CAMERA_ROTATE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_DOWN))
    camera_rotate_pitch(cam, -CAMERA_ROTATE_SPEED);
  
  if (keyboard_check_key_wo_conv(keys, KEYST_KEY_UP))
    camera_rotate_pitch(cam, CAMERA_ROTATE_SPEED);
  
  if (keys & ((1 << KEYST_KEY_W) | (1 << KEYST_KEY_A) | (1 << KEYST_KEY_RIGHT) | 
              (1 << KEYST_KEY_D) | (1 << KEYST_KEY_C) | (1 << KEYST_KEY_SPACE) | 
              (1 << KEYST_KEY_Q) | (1 << KEYST_KEY_E) | (1 << KEYST_KEY_LEFT) |
              (1 << KEYST_KEY_S) | (1 << KEYST_KEY_UP) | (1 << KEYST_KEY_DOWN)))
    apply_camera_view(cam);
}
