#include "action_layer.h"
#include "action_util.h"
#include "eeconfig.h"
#include "ergodox.h"
#include "keymap_colemak.h"
#include "version.h"

/* Aliases */
#define _______ KC_TRNS

/* Layers */
enum
{
  BASE = 0,
  NUMFUNC, // Left Hand 2 rows of fn keys, right hand 10 key
  MEDIA,   // Quick change of thumbs to control media playback
  MOBA,    // Left hand MOBA settings
  RGBW,    // Controls the shines underlighting
  CTRL,    // Things like rebooting the board to be flashed
};

/* Macros */
enum
{
  NONE = 0,

  // German Chars
  DE_A,  //
  DE_AA, // Ä
  DE_O,  //
  DE_OE, // Ö
  DE_SS, // ß
  DE_U,  //
  DE_UE, // Ü

  // Fx
  Fx,
};

/* Fn Keys */
enum
{
  F_BASE = 0,
  F_SHIFT,
  F_ALT,
  F_CTRL
};

/* Custom Keycodes */
enum
{
  CT_LBP,
  CT_RBP,

}

/* OS Type set by LEAD key + W/O/L. Used for OS dependent macros */
enum
{
  O_WIN = 0,
  O_OSX,
  O_LINUX,
}

/* set which ever is your default OS to true */
bool is_win = true;
bool is_osx = false;
bool is_linux = false;

bool time_travel = false;
bool skip_leds = false;

/* Keymaps */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

        /* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | ¯\_(ツ)_/¯ | 1  ! | 2  @ | 3  # | 4  $ | 5  % | '  " |          | -  _ | 6   ^ | 7  & | 8  * | 9  = | 0  + |          |
 * |           |      |      |      |      |      |      |           |      |       |      |      |     |      |            |
 * |-----------+------+------+------+------+-----+-------|           |------+------+------+------+------+------+-----------|
 * | (⌐■_■)    |   Q  |   W  |   F  |   P  |   G  |    ( |           |  )   |   J  |   L  |   U  |   Y  |     |            |
 * |-----------+------+------+------+------+------| [  { |           |  ]  }|------+------+------+------+------+-----------|
 * | ಠ_ಠ      | A  Ä |   R  | S  ß |   T  |   D  |------|           |------|   H  |   N  |   E  |   I  | O Ö |             |
 * |-----------+------+------+------+------+------|    ; |           | :     |------+------+------+------+------+-----------|
 * | ┻━┻    |  Z   |   X  |   C  |   V  |   B  | ,  < |           | .  > |   K  |   M  |  /  ? | \ | |     |             |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     | LEAD  |      |      |      |      |                                       |      |      |      |      |       |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | ESC  | Ctrl |           |      | Del  |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Alt |            |      |      |      |
 *                                  |Backsp| Shift|------|           |------| Enter| Space|
 *                                  |      |      | GUI  |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */

        [BASE] = KEYMAP(

            )

}
