#include "action_layer.h"
#include "action_util.h"
#include "eeconfig.h"
#include "ergodox.h"
#include "version.h"

/* Aliases */
#define _______ KC_TRNS

/* Layers */
enum
{
  BASE = 0,
  TNUM,  // Toggle top row numbers vs. symbols
  NUMP,  // right hand 10 key
  FUNC,  // Function keys
  MEDIA, // Media controls on right thumb cluster
  DOTA,  // Left hand MOBA settings (Geared towards DOTA2 for me)
  FPS,   // Left hand FPS settings (Geared towards BF1 for me)
  CONF,  // Things like rebooting the board to be flashed. NUM + FUNC
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
  CT_PLS,
  CT_EQ,
  CT_SHRUG,
  CT_YEAAAAH,
  CT_DSAPPRV,
  CT_TBL,
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
         * |           |      |      |      |      |      |  `   |           | ~    |       |      |      |      |      |  VOLUP   |
         * |-----------+------+------+------+------+-----+-------|           |------+------+------+------+------+------+-----------|
         * | (⌐■_■)    |   Q  |   W  |   F  |   P  |   G  |    ( |           |  )   |   J  |   L  |   U  |   Y  | ESC  |  VOLDN    |
         * |-----------+------+------+------+------+------| [  { |           |  ]  }|------+------+------+------+------+-----------|
         * | ಠ_ಠ      | A  Ä |   R  | S  ß |   T  |   D  |------|           |------|   H  |   N  |   E  |   I  | O Ö   |  MUTE     |
         * |-----------+------+------+------+------+------|    ; |           | :     |------+------+------+------+------+-----------|
         * | ┻━┻    |  Z   |   X  |   C  |   V  |   B  | ,  < |           | .  > |   K  |   M  |  /  ? | \ |  |   ^  |  PLAY/PS  |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     |  CONF | DOTA |  FPS | GUI  | LEAD |                                       | Home | End  |   <  |   v  |   >   |
         *     `-----------------------------------'                                       `-----------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | MEDIA| TNUM |           | ALT  | CTRL|
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | FUNC |           | Del  |      |      |
         *                                  |Backsp| Shift|------|           |------| Enter| Space|
         *                                  |      |      | NUMP |           | Tab  |      |      |
         *                                  `--------------------'           `--------------------'
         */

        [BASE] = KEYMAP(
            M(CT_SHRUG), KC_1, KC_2, KC_3, KC_4, KC_5, TD(CT_BTK),
            M(CT_YEEAH), KC_Q, KC_W, KC_F, KC_P, KC_G, TD(CT))

}
