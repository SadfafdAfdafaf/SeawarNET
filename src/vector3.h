#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include <math.h>

#ifndef VEC_EPS
  #define VEC_EPS 1e-5
#endif

#ifndef VECTOR_COORD
  typedef float vcoord_t;
  #define VECTOR_COORD
#endif

typedef union {
  vcoord_t c[3];
  struct {
    vcoord_t x, y, z;
  };
  struct {
    vcoord_t r, g, b;
  };
  struct {
    vcoord_t u, v, w;
  };
  struct {
    vcoord_t s, t, p;
  };
} vec3_t;

typedef vec3_t point3_t;
typedef vec3_t color_rgb_t;
typedef vec3_t texcoord3_t;
typedef vec3_t uvw_t;

#define vec3(a, b, c) (vec3_t){{(a), (b), (c)}}
#define point3(a, b, c) vec3(a, b, c)
#define color_rgb(a, b, c) vec3(a, b, c)
#define texcoord3(a, b, c) vec3(a, b, c)
#define uvw(a, b, c) vec3(a, b, c)

static inline vec3_t vec3_add(vec3_t a, vec3_t b) {
  return vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

static inline vec3_t vec3_sub(vec3_t a, vec3_t b) {
  return vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}

static inline vec3_t vec3_mul(vec3_t a, vec3_t b) {
  return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}

static inline vec3_t vec3_div(vec3_t a, vec3_t b) {
  return vec3(a.x / b.x, a.y / b.y, a.z / b.z);
}

static inline vec3_t vec3_scl(vec3_t a, vcoord_t b) {
  return vec3(a.x * b, a.y * b, a.z * b);
}

static inline vec3_t vec3_rdc(vec3_t a, vcoord_t b) {
  return vec3(a.x / b, a.y / b, a.z / b);
}

static inline vec3_t vec3_ird(vec3_t a, vcoord_t b) {
  return vec3(b / a.x, b / a.y, b / a.z);
}

static inline vec3_t vec3_neg(vec3_t a) {
  return vec3(-a.x, -a.y, -a.z);
}

static inline vec3_t vec3_addp(vec3_t *a, vec3_t *b) {
  return vec3(a->x + b->x, a->y + b->y, a->z + b->z);
}

static inline vec3_t vec3_subp(vec3_t *a, vec3_t *b) {
  return vec3(a->x - b->x, a->y - b->y, a->z - b->z);
}

static inline vec3_t vec3_mulp(vec3_t *a, vec3_t *b) {
  return vec3(a->x * b->x, a->y * b->y, a->z * b->z);
}

static inline vec3_t vec3_divp(vec3_t *a, vec3_t *b) {
  return vec3(a->x / b->x, a->y / b->y, a->z / b->z);
}

static inline vec3_t vec3_sclp(vec3_t *a, vcoord_t b) {
  return vec3(a->x * b, a->y * b, a->z * b);
}

static inline vec3_t vec3_rdcp(vec3_t *a, vcoord_t b) {
  return vec3(a->x / b, a->y / b, a->z / b);
}

static inline vec3_t vec3_irdp(vec3_t *a, vcoord_t b) {
  return vec3(b / a->x, b / a->y, b / a->z);
}

static inline vec3_t vec3_negp(vec3_t *a) {
  return vec3(-a->x, -a->y, -a->z);
}

static inline vec3_t vec3_rotp(vec3_t *a, 
                               vec3_t *b, 
                               vcoord_t c) {
  vcoord_t sn = sin(c);
  vcoord_t cs = cos(c);
  vcoord_t nc = 1.0 - cs;
  
  vcoord_t ax = a->x;
  vcoord_t ay = a->y;
  vcoord_t az = a->z;
  vcoord_t x = b->x;
  vcoord_t y = b->y;
  vcoord_t z = b->z;
  
  vcoord_t ncxy = nc * x * y;
  vcoord_t ncxz = nc * x * z;
  vcoord_t ncyz = nc * y * z;
  
  return vec3(ax * (cs + nc * x * x) +
              ay * (ncxy - sn * z) +
              az * (ncxz + sn * y),
              ax * (ncxy + sn * z) +
              ay * (cs + nc * y * y) +
              az * (ncyz - sn * x),
              ax * (ncxz - sn * y) +
              ay * (ncyz + sn * x) +
              az * (cs + nc * z * z));
}

static inline vec3_t vec3_crsp(vec3_t *a, vec3_t *b) {
  return vec3(a->y * b->z - a->z * b->y,
              a->z * b->x - a->x * b->z,
              a->x * b->y - a->y * b->x);
}

static inline double vec3_dotp(vec3_t *a, vec3_t *b) {
  return a->x * b->x + a->y * b->y + a->z * b->z;
}

static inline vec3_t vec3_rflp(vec3_t *a, vec3_t *b) {
  vec3_t r = vec3_sclp(b, 2.0 * vec3_dotp(a, b));
  
  return vec3_subp(a, &r);
}

static inline vec3_t vec3_rfrp(vec3_t *a, 
                               vec3_t *b, 
                               vcoord_t c) {
  double dot = vec3_dotp(a, b);
  double k = 1.0 - c * c * (1.0 - dot * dot);
  
  return (k >= 0.0) ? vec3_sub(vec3_sclp(a, c), 
                      vec3_sclp(b, c * dot + sqrt(k))) : vec3(0.0, 0.0, 0.0);
}

static inline double vec3_lenp(vec3_t *a) {
  vcoord_t x = a->x;
  vcoord_t y = a->y;
  vcoord_t z = a->z;
  
  return sqrt(x * x + y * y + z * z);
}

static inline double vec3_slnp(vec3_t *a) {
  vcoord_t x = a->x;
  vcoord_t y = a->y;
  vcoord_t z = a->z;
  
  return x * x + y * y + z * z;
}

static inline vec3_t vec3_rot(vec3_t a, 
                              vec3_t b, 
                              vcoord_t c) {
  vcoord_t sn = sin(c);
  vcoord_t cs = cos(c);
  vcoord_t nc = 1.0 - cs;
  
  vcoord_t ax = a.x;
  vcoord_t ay = a.y;
  vcoord_t az = a.z;
  vcoord_t x = b.x;
  vcoord_t y = b.y;
  vcoord_t z = b.z;
  
  vcoord_t ncxy = nc * x * y;
  vcoord_t ncxz = nc * x * z;
  vcoord_t ncyz = nc * y * z;
  
  return vec3(ax * (cs + nc * x * x) +
              ay * (ncxy - sn * z) +
              az * (ncxz + sn * y),
              ax * (ncxy + sn * z) +
              ay * (cs + nc * y * y) +
              az * (ncyz - sn * x),
              ax * (ncxz - sn * y) +
              ay * (ncyz + sn * x) +
              az * (cs + nc * z * z));
}

static inline vec3_t vec3_crs(vec3_t a, vec3_t b) {
  return vec3(a.y * b.z - a.z * b.y,
              a.z * b.x - a.x * b.z,
              a.x * b.y - a.y * b.x);
}

static inline double vec3_dot(vec3_t a, vec3_t b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline vec3_t vec3_rfl(vec3_t a, vec3_t b) {
  vec3_t *pa = &a;
  vec3_t *pb = &b;
  vec3_t r = vec3_sclp(pb, 2.0 * vec3_dotp(pa, pb));
  
  return vec3_subp(pa, &r);
}

static inline vec3_t vec3_rfr(vec3_t a, vec3_t b, vcoord_t c) {
  vec3_t *pa = &a;
  vec3_t *pb = &b;
  
  double dot = vec3_dotp(pa, pb);
  double k = 1.0 - c * c * (1.0 - dot * dot);
  
  return (k >= 0.0) ? vec3_sub(vec3_sclp(pa, c), 
                      vec3_sclp(pb, c * dot + sqrt(k))) : vec3(0.0, 0.0, 0.0);
}

static inline double vec3_len(vec3_t a) {
  vcoord_t x = a.x;
  vcoord_t y = a.y;
  vcoord_t z = a.z;
  
  return sqrt(x * x + y * y + z * z);
}

static inline double vec3_sln(vec3_t a) {
  vcoord_t x = a.x;
  vcoord_t y = a.y;
  vcoord_t z = a.z;
  
  return x * x + y * y + z * z;
}

static inline void vec3_addu(vec3_t *a, vec3_t b) {
  a->x += b.x;
  a->y += b.y;
  a->z += b.z;
}

static inline void vec3_subu(vec3_t *a, vec3_t b) {
  a->x -= b.x;
  a->y -= b.y;
  a->z -= b.z;
}

static inline void vec3_mulu(vec3_t *a, vec3_t b) {
  a->x *= b.x;
  a->y *= b.y;
  a->z *= b.z;
}

static inline void vec3_divu(vec3_t *a, vec3_t b) {
  a->x /= b.x;
  a->y /= b.y;
  a->z /= b.z;
}

static inline void vec3_sclu(vec3_t *a, vcoord_t b) {
  a->x *= b;
  a->y *= b;
  a->z *= b;
}

static inline void vec3_rdcu(vec3_t *a, vcoord_t b) {
  a->x /= b;
  a->y /= b;
  a->z /= b;
}

static inline void vec3_irdu(vec3_t *a, vcoord_t b) {
  a->x = b / a->x;
  a->y = b / a->y;
  a->z = b / a->z;
}

static inline void vec3_negu(vec3_t *a) {
  a->x *= -1.0;
  a->y *= -1.0;
  a->z *= -1.0;
}

static inline void vec3_addup(vec3_t *a, vec3_t *b) {
  a->x += b->x;
  a->y += b->y;
  a->z += b->z;
}

static inline void vec3_subup(vec3_t *a, vec3_t *b) {
  a->x -= b->x;
  a->y -= b->y;
  a->z -= b->z;
}

static inline void vec3_mulup(vec3_t *a, vec3_t *b) {
  a->x *= b->x;
  a->y *= b->y;
  a->z *= b->z;
}

static inline void vec3_divup(vec3_t *a, vec3_t *b) {
  a->x /= b->x;
  a->y /= b->y;
  a->z /= b->z;
}

static inline void vec3_rotu(vec3_t *a, 
                             vec3_t b, 
                             vcoord_t c) {
  vcoord_t sn = sin(c);
  vcoord_t cs = cos(c);
  vcoord_t nc = 1.0 - cs;

  vcoord_t ax = a->x;
  vcoord_t ay = a->y;
  vcoord_t az = a->z;
  vcoord_t x = b.x;
  vcoord_t y = b.y;
  vcoord_t z = b.z;
  
  vcoord_t ncxy = nc * x * y;
  vcoord_t ncxz = nc * x * z;
  vcoord_t ncyz = nc * y * z;
  
  a->x = ax * (cs + nc * x * x) +
         ay * (ncxy - sn * z) +
         az * (ncxz + sn * y),
  a->y = ax * (ncxy + sn * z) +
         ay * (cs + nc * y * y) +
         az * (ncyz - sn * x),
  a->z = ax * (ncxz - sn * y) +
         ay * (ncyz + sn * x) +
         az * (cs + nc * z * z);
}

static inline void vec3_rflu(vec3_t *a, vec3_t b) {
  vec3_t *pb = &b;
  vec3_t r = vec3_sclp(pb, 2.0 * vec3_dotp(a, pb));
  
  vec3_subup(a, &r);
}

static inline void vec3_rfru(vec3_t *a, vec3_t b, vcoord_t c) {
  vec3_t *pb = &b;
  
  double dot = vec3_dotp(a, pb);
  double k = 1.0 - c * c * (1.0 - dot * dot);
  
  *a = (k >= 0.0) ? vec3_sub(vec3_sclp(a, c), 
                    vec3_sclp(pb, c * dot + sqrt(k))) : vec3(0.0, 0.0, 0.0);
}

static inline void vec3_rotup(vec3_t *a, 
                              vec3_t *b, 
                              vcoord_t c) {
  vcoord_t sn = sin(c);
  vcoord_t cs = cos(c);
  vcoord_t nc = 1.0 - cs;
  
  vcoord_t ax = a->x;
  vcoord_t ay = a->y;
  vcoord_t az = a->z;
  vcoord_t x = b->x;
  vcoord_t y = b->y;
  vcoord_t z = b->z;
  
  vcoord_t ncxy = nc * x * y;
  vcoord_t ncxz = nc * x * z;
  vcoord_t ncyz = nc * y * z;
  
  a->x = ax * (cs + nc * x * x) +
         ay * (ncxy - sn * z) +
         az * (ncxz + sn * y),
  a->y = ax * (ncxy + sn * z) +
         ay * (cs + nc * y * y) +
         az * (ncyz - sn * x),
  a->z = ax * (ncxz - sn * y) +
         ay * (ncyz + sn * x) +
         az * (cs + nc * z * z);
}

static inline void vec3_rflup(vec3_t *a, vec3_t *b) {
  vec3_subu(a, vec3_sclp(b, 2.0 * vec3_dotp(a, b)));
}

static inline void vec3_rfrup(vec3_t *a, 
                              vec3_t *b, 
                              vcoord_t c) {
  double dot = vec3_dotp(a, b);
  double k = 1.0 - c * c * (1.0 - dot * dot);
  
  *a = (k >= 0.0) ? vec3_sub(vec3_sclp(a, c), 
                    vec3_sclp(b, c * dot + sqrt(k))) : vec3(0.0, 0.0, 0.0);
}

static inline vec3_t vec3_nrmp(vec3_t *a) {
  return vec3_rdcp(a, vec3_lenp(a));
}

static inline vec3_t vec3_nrm(vec3_t a) {
  return vec3_nrmp(&a);
}

static inline void vec3_nrmu(vec3_t *a) {  
  vec3_rdcu(a, vec3_lenp(a));
}

#endif