#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector3.h"

typedef struct {
  vec3_t pos;
  
  vec3_t fwd;
  vec3_t up;
  vec3_t rgt;
  
  float fov;
  float aspect;
  float znear;
  float zfar;
} camera_t;

static inline void camera_init(camera_t *cam, vec3_t pos, vec3_t fwd, vec3_t up, 
                               float fov, float aspect, float zn, float zf) {
  vec3_t nfwd = vec3_nrm(fwd);
  vec3_t nup = vec3_nrm(up);
  
  *cam = (camera_t){
    .pos = pos,
    .fwd = nfwd,
    .up = nup,
    .rgt = vec3_crsp(&nfwd, &nup),
    
    .fov = fov,
    .aspect = aspect,
    .znear = zn,
    .zfar = zf,
  };
}

static inline void camera_move_fwd(camera_t *cam, float u) {
  vec3_addu(&cam->pos, vec3_sclp(&cam->fwd, u));
}

static inline void camera_move_back(camera_t *cam, float u) {
  vec3_subu(&cam->pos, vec3_sclp(&cam->fwd, u));
}

static inline void camera_move_right(camera_t *cam, float u) {
  vec3_addu(&cam->pos, vec3_sclp(&cam->rgt, u));
}

static inline void camera_move_left(camera_t *cam, float u) {
  vec3_subu(&cam->pos, vec3_sclp(&cam->rgt, u));
}

static inline void camera_move_up(camera_t *cam, float u) {
  vec3_addu(&cam->pos, vec3_sclp(&cam->up, u));
}

static inline void camera_move_down(camera_t *cam, float u) {
  vec3_subu(&cam->pos, vec3_sclp(&cam->up, u));
}

static inline void camera_move_up_y(camera_t *cam, float u) {
  vec3_addu(&cam->pos, vec3(0.0, 1.0, 0.0));
}

static inline void camera_move_down_y(camera_t *cam, float u) {
  vec3_subu(&cam->pos, vec3(0.0, 1.0, 0.0));
}

static inline void camera_rotate_yaw(camera_t *cam, float u) {
  vec3_rotup(&cam->fwd, &cam->up, u);
  vec3_nrmu(&cam->fwd);
  vec3_rotup(&cam->rgt, &cam->up, u);
  vec3_nrmu(&cam->rgt);
}

static inline void camera_rotate_pitch(camera_t *cam, float u) {
  vec3_rotup(&cam->fwd, &cam->rgt, u);
  vec3_nrmu(&cam->fwd);
  vec3_rotup(&cam->up, &cam->rgt, u);
  vec3_nrmu(&cam->up);
}

static inline void camera_rotate_roll(camera_t *cam, float u) {
  vec3_rotup(&cam->rgt, &cam->fwd, u);
  vec3_nrmu(&cam->rgt);
  vec3_rotup(&cam->up, &cam->fwd, u);
  vec3_nrmu(&cam->up);
}

static inline void camera_rotate_up(camera_t *cam, float u) {
  vec3_t up = vec3(0.0, 1.0, 0.0);
  
  vec3_rotup(&cam->fwd, &up, u);
  vec3_nrmu(&cam->fwd);
  vec3_rotup(&cam->up, &up, u);
  vec3_nrmu(&cam->up);
  vec3_rotup(&cam->rgt, &up, u);
  vec3_nrmu(&cam->rgt);
}

static inline void camera_set_fov(camera_t *cam, float u) {
  cam->fov = u;
}

static inline void camera_set_aspect(camera_t *cam, float u) {
  cam->aspect = u;
}

static inline void camera_set_znear(camera_t *cam, float u) {
  cam->znear = u;
}

static inline void camera_set_zfar(camera_t *cam, float u) {
  cam->zfar = u;
}

#endif