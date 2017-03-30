#include "action_layer.h"
#include "action_util.h"
#include "eeconfig.h"
#include "ergodox.h"
#include "keymap_colemak.h"
#include "version.h"

/* Layers */
enum {
  BASE = 0,
  NUMFUNC,  // Left Hand: 2 rows of fn keys, right hand 10 key
  MEDIA,    // Quick change of thumbs to control media playback
  RGBW,     // Controls the shines underlighting
  CTRL,     // Things like rebooting the board to be flashed
};

/* Macros */
enum {
  NONE = 0,

  // German Chars
  DE_AA,  // Ä
  DE_OE,  // Ö
  DE_SS,  // ß
  DE_UE,  // Ü

  // Fx
  Fx,
};

/* Fn Keys */
enum { F_BASE = 0, F_SHIFT, F_ALT, F_CTRL };

/* Custom Keycodes */
enum {
  CT_LBP,
  CT_RBP,

}
