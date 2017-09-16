#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

#include <SDL.h>

#define KEY_MOVE_FWD SDLK_w
#define KEY_MOVE_BACK SDLK_s
#define KEY_MOVE_LEFT SDLK_a
#define KEY_MOVE_RIGHT SDLK_d
#define KEY_MOVE_UP SDLK_SPACE
#define KEY_MOVE_DOWN SDLK_c

#define KEY_ROTATE_ROLL_LEFT SDLK_q
#define KEY_ROTATE_ROLL_RIGHT SDLK_e
#define KEY_ROTATE_YAW_LEFT SDLK_LEFT
#define KEY_ROTATE_YAW_RIGHT SDLK_RIGHT
#define KEY_ROTATE_PITCH_UP SDLK_UP
#define KEY_ROTATE_PITCH_DOWN SDLK_DOWN

typedef int keystate_t;

enum keystate_key_t {
  KEYST_KEY_W,
  KEYST_KEY_A,
  KEYST_KEY_S,
  KEYST_KEY_D,
  KEYST_KEY_SPACE,
  KEYST_KEY_C,
  KEYST_KEY_Q,
  KEYST_KEY_E,
  KEYST_KEY_LEFT,
  KEYST_KEY_RIGHT,
  KEYST_KEY_UP,
  KEYST_KEY_DOWN,
};

static inline void keyboard_init(keystate_t *state) {
  *state = 0;
}

static inline int keyboard_get_key_pos(int key) {
  switch (key) {
    case KEY_MOVE_FWD:
      return KEYST_KEY_W;
    case KEY_MOVE_BACK:
      return KEYST_KEY_S;
    case KEY_MOVE_LEFT:
      return KEYST_KEY_A;
    case KEY_MOVE_RIGHT:
      return KEYST_KEY_D;
    case KEY_MOVE_UP:
      return KEYST_KEY_SPACE;
    case KEY_MOVE_DOWN:
      return KEYST_KEY_C;
      
    case KEY_ROTATE_ROLL_LEFT:
      return KEYST_KEY_Q;
    case KEY_ROTATE_ROLL_RIGHT:
      return KEYST_KEY_E;
    case KEY_ROTATE_YAW_LEFT:
      return KEYST_KEY_LEFT;
    case KEY_ROTATE_YAW_RIGHT:
      return KEYST_KEY_RIGHT;
    case KEY_ROTATE_PITCH_UP:
      return KEYST_KEY_UP;
    case KEY_ROTATE_PITCH_DOWN:
      return KEYST_KEY_DOWN;
  };
  
  return -1;
}

static inline void keyboard_set_key_wo_conv(keystate_t *state, int key_pos) {
  *state |= 1 << key_pos;
}

static inline void keyboard_clear_key_wo_conv(keystate_t *state, int key_pos) {
  *state &= ~(1 << key_pos);
}

static inline int keyboard_check_key_wo_conv(keystate_t state, int key_pos) {
  return state & (1 << key_pos);
}

static inline void keyboard_set_key(keystate_t *state, int key) {
  int key_pos = keyboard_get_key_pos(key);
  
  if (key_pos == -1)
    return;
  
  keyboard_set_key_wo_conv(state, key_pos);
}

static inline void keyboard_clear_key(keystate_t *state, int key) {
  int key_pos = keyboard_get_key_pos(key);
  
  if (key_pos == -1)
    return;
  
  keyboard_clear_key_wo_conv(state, key_pos);
}

static inline int keyboard_check_key(keystate_t state, int key) {
  int key_pos = keyboard_get_key_pos(key);
  
  if (key_pos == -1)
    return 0;
  
  return keyboard_check_key_wo_conv(state, key_pos);
}


#endif