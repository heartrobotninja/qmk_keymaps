#include "action_layer.h"
#include "action_util.h"
#include "debug.h"
#include "eeconfig.h"
#include "ergodox.h"
#include "ez.h"
#include "version.h"

/* Aliases */
#define ---- KC_TRNS

/* Layers */
enum
{
  BASE = 0,
  TSYM,  // Toggle top row numbers vs. symbols
  NUMP,  // right hand 10 key
  FUNC,  // Function keys
  MDIA, // Media controls on right thumb cluster
  MOBA,  // Left hand MOBA settings (Geared towards DOTA2 for me)
  FPS,   // Left hand FPS settings (Geared towards BF1 for me)
  CONF,  // Things like rebooting the board to be flashed. NUM + FUNC
};

/* Macros */
enum
{
  NONE = 0,

  // Random Ascii Emoji
  EM_HAPPY,
  EM_SAD,
  EM_INDF,
  EM_MAD,

  // Media Macros
  MD_MUSIC,
  MD_VIDEO,
  MD_PREV,
  MD_NEXT,

  // Custom Shift Numbers
  NM_9,  // Has shifted equals
  NM_0,  // Has shifted plus

  // Config Macros
  CF_EPRM,
  CF_VERS,

  // RGB Macro
  RGB_ANI,
};

/* Tap Dancery */
enum
{
  TD_BTK,
  TD_TDE,
  TD_LPRN,
  TD_RPRN,
  TD_SCLN,
  TD_COLN,
  TD_COPY,
  TD_UNDO,
};

/* OS Identifier */
enum
{
  OS_WIN = 0,
  OS_OSX, 
  OS_LIN,
};

uint8_t os_type = OS_WIN;

static uint16_t rgb_timer;
static uint16_t em_timer;
bool time_travel = false;
bool skip_leds = false;

/* Keymaps */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

        /* Keymap 0: Base Layer
         *
         * ,-----------------------------------------------------.           ,-----------------------------------------------------.
         * |   Un/Redo | 1  ! | 2  @ | 3  # | 4  $ | 5  % | '  " |           | -  _ | 6   ^ | 7  & | 8  * | 9  = | 0  + |          |
         * |           |      |      |      |      |      |  `   |           | ~    |       |      |      |      |      |  VOLUP   |
         * |-----------+------+------+------+------+------+------|           |------+------+------+------+------+------+-----------|
         * |   CCopy   |   Q   |   W  |   F  |   P  |  G  |     ( |           |  )   |   J  |   L  | U Ü  |   Y  | ESC  |  VOLDN    |
         * |-----------+------+------+------+------+------| [  { |           |  ]  }|------+------+------+------+------+-----------|
         * |  Paste    | A  Ä  |   R  | S  ß |   T  |   D  |------|          |------|   H  |   N  |   E  |   I  | O Ö   |  MUTE     |
         * |-----------+------+------+------+------+------|    ; |           | :     |------+------+------+------+------+-----------|
         * |  Find     |  Z   |   X  |   C  |   V  |   B  | ,  < |          | .  > |   K  |   M  |  /  ? | \ |  |   ^  |  PLAY/PS  |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     |  CONF | MOBA |  FPS | GUI  | LEAD |                                       | Home | End  |   <  |   v  |   >   |
         *     `-----------------------------------'                                       `-----------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | MEDIA| TSYM |           | ALT  | CTRL|
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | FUNC |           | Del  |      |      |
         *                                  |Backsp| Shift|------|           |------| Enter| Space|
         *                                  |      |      | NUMP |           | Tab  |      |      |
         *                                  `--------------------'           `--------------------'
         */

        [BASE] = KEYMAP(
            // Left Hand
            TD(TD_UNDO),  KC_1,     KC_2,    KC_3,          KC_4,    KC_5, TD(TD_BTK),
            TD(TD_COPY),  KC_Q,     KC_W,    KC_F,          KC_P,    KC_G, TD(TD_LPRN),
            KC_PASTE,     KC_A,     KC_R,    KC_S,          KC_T,    KC_D,
            KC_FIND,      KC_Z,     KC_X,    KC_C,          KC_V,    KC_B, TD(TD_SCLN),
            MO(CONF),     TG(DOTA), TG(FPS), OSM(MOD_LGUI), KC_LEAD,
                                                                                        MO(MDIA), TG(TSYM),
                                                                                                  TT(FUNC),
                                                                          KC_BSPC, OSM(MOD_LSFT), TT(NUMP),

            // Right Hand
                        TD(TD_TDE),  KC_6, KC_7,    KC_8,    M(NM_9), M(NM_0),  KC_VOLU,
                        TD(TD_RPRN), KC_J, KC_L,    KC_U,    KC_Y,    KC_ESC,   KC_VOLD,
                                     KC_H, KC_N,    KC_E,    KC_I,    KC_O,     KC_MUTE,
                        TD(TD_COLN), KC_K, KC_M,    KC_SLSH, KC_BSLS, KC_UP,    KC_MPLY,
                                           KC_HOME, KC_END,  KC_LEFT, KC_DOWN,  KC_RGHT,
            OSM(MOD_RALT), OSM(MOD_RCTL),
            KC_DEL,
            KC_TAB, KC_ENT, KC_SPC
        ),

        /* Keymap 1: TSYM Layer
         *
         * ,-----------------------------------------------------.           ,-----------------------------------------------------.
         * |   ----    |  !   |  @   |  #   |   $  |   %  |      |           |      |   ^   |  &   |  *   |  =   |  +   |          |
         * |           |      |      |      |      |      | ---- |           | ---- |       |      |      |      |      |   ----   |
         * |-----------+------+------+------+------+------+------|           |------+------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |      |           |      | ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------| ---- |           | ---- |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |------|           |------| ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- | ---- | ---- | ---- | ---- | ---- |   ----    |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     | ----  | ---- | ---- | ---- | ---- |                                       | ---- | ---- | ---- | ---- | ----  |
         *     `-----------------------------------'                                       `-----------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | ---- | ---- |           | ---- | ---- |
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  | ---- | ---- |------|           |------| ---- | ---- |
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  `--------------------'           `--------------------'
         */

        [TSYM] = KEYMAP(
          // Left Hand
            ----, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, ----,
            ----, ----,    ----,  ----,    ----,   ----,    ----,
            ----, ----,    ----,  ----,    ----,   ----,
            ----, ----,    ----,  ----,    ----,   ----,    ----,
            ----, ----,    ----,  ----,    ----,
                                                                          ----, ----,
                                                                                ----,
                                                                    ----, ----, ----,

            // Right Hand
                          ----, KC_CIRC, KC_AMPR, KC_ASTR, KC_EQL, KC_PLUS, ----,
                          ----, ----,    ----,    ----,    ----,   ----,    ----,
                                ----,    ----,    ----,    ----,   ----,    ----,
                          ----, ----,    ----,    ----,    ----,   ----,    ----,
                                         ----,    ----,    ----,   ----,    ----,
            ----, ----,
            ----,
            ----, ----, ----
        ),

        /* Keymap 2: NUMP Layer
         *
         * ,-----------------------------------------------------.           ,-----------------------------------------------------.
         * |   ----    | ---- | ---- | ---- | ---- | ---- |      |           |      |   (  |   )  |   ^  |   /  | ---- |           |
         * |           |      |      |      |      |      | ---- |           | ---- |      |      |      |      |      |   ----    |
         * |-----------+------+------+------+------+------+------|           |------+------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |      |           |      |   7  |   8  |   9  |   *  | ---- |   ----    |
         * |-----------+------+------+------+------+------| ---- |           | ---- |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |------|           |------|   4  |   5  |   6  |   -  | ---- |   ----    |
         * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- |   1  |   2  |   3  |   +  | ---- |   ----    |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     | ----  | ---- | ---- | ---- | ---- |                                       |   0  |   =  | ---- | ---- | ----  |
         *     `-----------------------------------'                                       `-------------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | ---- | ---- |           | ---- | ---- |
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  | ---- | ---- |------|           |------| ---- | ---- |
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  `--------------------'           `--------------------'
         */
        [NUMP] = KEYMAP(
          // Left Hand
            ----, ----, ----, ----, ----, ----, ----,
            ----, ----, ----, ----, ----, ----, ----,
            ----, ----, ----, ----, ----, ----,
            ----, ----, ----, ----, ----, ----, ----,
            ----, ----, ----, ----, ----,
                                                      ----, ----,
                                                            ----,
                                                ----, ----, ----,

            // Right Hand
                        ----, KC_LPRN, KC_RPRN, KC_CIRC, KC_PSLS, ----, ----,
                        ----, KC_7,    KC_8,    KC_9,    KC_PAST, ----, ----,
                              KC_4,    KC_5,    KC_6,    KC_PMNS, ----, ----,
                        ----, KC_1,    KC_2,    KC_3,    KC_PPLS, ----, ----,
                                       KC_0,    KC_PEQL, ----,    ----, ----,
            ----, ----,
            ----,
            ----, ----, ----
        ),

        /* Keymap 3: FUNC Layer
         *
         * ,-----------------------------------------------------.           ,-----------------------------------------------------.
         * |   ----    |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |           |  F7  |  F8  |  F9  |  F10 |  F11 |  F12 |           |
         * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |   ----    |
         * |-----------+------+------+------+------+------+------|           |------+------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |      |           |      | ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------| ---- |           | ---- |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |------|           |------| ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- | ---- | ---- | ---- | ---- | ---- |   ----    |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     | ----  | ---- | ---- | ---- | ---- |                                       | ---- | ---- | ---- | ---- | ----  |
         *     `-----------------------------------'                                       `-----------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | ---- | ---- |           | ---- | ---- |
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  | ---- | ---- |------|           |------| ---- | ---- |
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  `--------------------'           `--------------------'
         */
        [FUNC] = KEYMAP(
          // Left Hand
            ----, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,
            ----, ----,  ----,  ----,  ----,  ----,  ----,
            ----, ----,  ----,  ----,  ----,  ----,
            ----, ----,  ----,  ----,  ----,  ----,  ----,
            ----, ----,  ----,  ----,  ----,
                                                           ----, ----,
                                                                 ----,
                                                     ----, ----, ----,

            // Right Hand
                        ----, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12,
                        ----, ----,  ----,  ----,  ----,   ----,   ----,
                              ----,  ----,  ----,  ----,   ----,   ----,
                        ----, ----,  ----,  ----,  ----,   ----,   ----,
                                     ----,  ----,  ----,   ----,   ----,
            ----, ----,
            ----,
            ----, ----, ----
        ),

        /* Keymap 4: MDIA Layer
         *
         * ,-----------------------------------------------------.           ,-----------------------------------------------------.
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- | ---- | ---- | ---- | ---- | ---- |           |
         * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |   MUSIC   |
         * |-----------+------+------+------+------+------+------|           |------+------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |      |           |      | ---- | ---- | ---- | ---- | ---- |   VIDEO   |
         * |-----------+------+------+------+------+------| ---- |           | ---- |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |------|           |------| ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- | ---- | ---- | ---- | ---- | ---- |   ----    |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     | ----  | ---- | ---- | ---- | ---- |                                       | MUTE | ---- | ---- | ---- | ----  |
         *     `-----------------------------------'                                       `-----------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | ---- | ---- |           | PREV | NEXT |
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | ---- |           | VOLU |      |      |
         *                                  | ---- | ---- |------|           |------| PLAY | STOP |
         *                                  |      |      | ---- |           | VOLD |      |      |
         *                                  `--------------------'           `--------------------'
         */
        [MDIA] = KEYMAP(
          // Left Hand
            ----, ----,  ----,  ----,  ----,  ----,  ----,
            ----, ----,  ----,  ----,  ----,  ----,  ----,
            ----, ----,  ----,  ----,  ----,  ----,
            ----, ----,  ----,  ----,  ----,  ----,  ----,
            ----, ----,  ----,  ----,  ----,
                                                           ----, ----,
                                                                 ----,
                                                     ----, ----, ----,

            // Right Hand
                        ----, ----,  ----,     ----,  ----, ----, M(MD_MUSIC),
                        ----, ----,  ----,     ----,  ----, ----, M(MD_VIDEO),
                              ----,  ----,     ----,  ----, ----, ----,
                        ----, ----,  ----,     ----,  ----, ----, ----,
                                     KC_MUTE,  ----,  ----, ----, ----,
            M(MD_PREV), M(MD_NEXT),
            KC_VOLU,
            KC_VOLD, KC_MPLY, KC_MSTP
        ),

        /* Keymap 5: MOBA Layer (Meant to put things easily on the left hand)
         *
         * ,-----------------------------------------------------.           ,-----------------------------------------------------.
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- | ---- | ---- | ---- | ---- | ---- |           |
         * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |   ----    |
         * |-----------+------+------+------+------+------+------|           |------+------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |      |           |      | ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------| ---- |           | ---- |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |------|           |------| ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- | ---- | ---- | ---- | ---- | ---- |   ----    |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     | ----  | ---- | ---- | ---- | ---- |                                       | ---- | ---- | ---- | ---- | ----  |
         *     `-----------------------------------'                                       `-----------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | ---- | ---- |           | ---- | ---- |
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  | ---- | ---- |------|           |------| ---- | ---- |
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  `--------------------'           `--------------------'
         */
        [MOBA] = KEYMAP(),

        /* Keymap 6: FPS Layer (Meant to put things easily on the left hand)
         *
         * ,-----------------------------------------------------.           ,-----------------------------------------------------.
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- | ---- | ---- | ---- | ---- | ---- |           |
         * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |   ----    |
         * |-----------+------+------+------+------+------+------|           |------+------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |      |           |      | ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------| ---- |           | ---- |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |------|           |------| ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | ---- | ---- | ---- | ---- | ---- | ---- |   ----    |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     | ----  | ---- | ---- | ---- | ---- |                                       | ---- | ---- | ---- | ---- | ----  |
         *     `-----------------------------------'                                       `-----------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | ---- | ---- |           | ---- | ---- |
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  | ---- | ---- |------|           |------| ---- | ---- |
         *                                  |      |      | ---- |           | ---- |      |      |
         *                                  `--------------------'           `--------------------'
         */
        [FPS] = KEYMAP(),

        /* Keymap 7: Configuration Layer
         *
         * ,-----------------------------------------------------.           ,-----------------------------------------------------.
         * |  EEPROM   | ---- | ---- | ---- | ---- | ---- | ---- |           | PWR  | ---- | ---- | ---- | ---- | ---- |           |
         * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |  VERSION  |
         * |-----------+------+------+------+------+------+------|           |------+------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |      |           |      | ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------| ---- |           | SLP |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- |------|           |------| ---- | ---- | ---- | ---- | ---- |   ----    |
         * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
         * |   ----    | ---- | ---- | ---- | ---- | ---- | ---- |           | WAKE | ---- | ---- | ---- | ---- | ---- |   ----    |
         * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
         *     | ----  | ---- | ---- | ---- | ---- |                                       | ---- | ---- | ---- | ---- | ----  |
         *     `-----------------------------------'                                       `-----------------------------------'
         *                                         ,-------------.           ,-------------.
         *                                         | ---- | ---- |           | TOG  | ANI  |
         *                                  ,------|------|------|           |------+------+------.
         *                                  |      |      | ---- |           | VAI |      |      |
         *                                  | ---- | ---- |------|           |------| HUI  | HUID |
         *                                  |      |      | ---- |           | VAD |      |      |
         *                                  `--------------------'           `--------------------'
         */
        [CONF] = KEYMAP(
          // Left Hand
            M(CF_EPRM), ----,  ----,  ----,  ----,  ----,  KC_PWR,
            ----,       ----,  ----,  ----,  ----,  ----,  KC_SLEP,
            ----,       ----,  ----,  ----,  ----,  ----,
            ----,       ----,  ----,  ----,  ----,  ----,  KC_WAKE,
            ----,       ----,  ----,  ----,  ----,
                                                           ----, ----,
                                                                 ----,
                                                     ----, ----, ----,

            // Right Hand
                        KC_PWR,  ----,  ----, ----,  ----, ----, M(CF_VERS),
                        KC_SLEP, ----,  ----, ----,  ----, ----, ----,
                                 ----,  ----, ----,  ----, ----, ----,
                        KC_WAKE, ----,  ----, ----,  ----, ----, ----,
                                        ----, ----,  ----, ----, ----,
            RGB_TOG, M(RGB_ANI),
            RGB_VAI,
            RGB_VAD, RGB_HUI, RGB_HUD
        ),
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [TD_BTK] = ACTION_TAP_DANCE_DOUBLE(KC_QUOT, KC_GRV),
  [TD_TDE] = ACTION_TAP_DANCE_DOUBLE(KC_MINS, KC_TILD),
  [TD_LPRN] = ACTION_TAP_DANCE_DOUBLE(KC_LBRC, KC_LPRN),
  [TD_RPRN] = ACTION_TAP_DANCE_DOUBLE(KC_RBRC, KC_RPRN),
  [TD_SCLN] = ACTION_TAP_DANCE_DOUBLE(KC_COMM, KC_SCLN),
  [TD_COLN] = ACTION_TAP_DANCE_DOUBLE(KC_DOT, KC_COLN),
  [TD_COPY] = ACTION_TAP_DANCE_DOUBLE(KC_COPY, KC_CUT),
  [TD_UNDO] = ACTION_TAP_DANCE_DOUBLE(KC_UNDO, KC_REDO),
};

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt){
  switch(id) {
    case NM_0:
      if(record->event.pressed){
        if(keyboard_report->mods & MOD_BIT(KC_LSFT)){
          unregister_code(KC_LSFT);
          register_code(KC_PLUS);
          unregister_code(KC_PLUS);
          register_code(KC_LSFT);
        } else{
          register_code(KC_0);
          unregister_code(KC_0);
        }
      }
      break;
    case NM_9:
      if(record->event.pressed){
        if(keyboard_report->mods & MOD_BIT(KC_LSFT)){
          unregister_code(KC_LSFT);
          register_code(KC_EQL);
          unregister_code(KC_EQL);
          register_code(KC_LSFT);
        } else{
          register_code(KC_9);
          unregister_code(KC_9);
        }
      }
      break;
    case MD_MUSIC:
      if(record->event.pressed){
        switch(os_type) {
          case OS_WIN:
            register_code(KC_LCTL);
            register_code(KC_LALT);
            register_code(KC_G);
            unregister_code(KC_G);
            unregister_code(KC_LALT);
            unregister_code(KC_LCTL);
            break;
          case OS_OSX: break;
          case OS_LIN: break;
        }
      }
      break;
    case MD_VIDEO:
      if(record->event.pressed){
        switch(os_type) {
          case OS_WIN:
            register_code(KC_LCTL);
            register_code(KC_LALT);
            register_code(KC_V);
            unregister_code(KC_V);
            unregister_code(KC_LALT);
            unregister_code(KC_LCTL);
            break;
          case OS_OSX: break;
          case OS_LIN: break;
        }
      }
      break;
    case MD_PREV:
      if(record->event.pressed){
        switch(os_type) {
          case OS_OSX:
            register_code(KC_MRWD);
            unregister_code(KC_MRWD);
            break;
          default:
            register_code(KC_MPRV);
            unregister_code(KC_MPRV);
        }
      }
    case MD_NEXT:
      if(record->event.pressed){
        switch(os_type) {
          case OS_OSX:
            register_code(KC_MFFD);
            unregister_code(KC_MFFD);
            break;
          default:
            register_code(KC_MNXT);
            unregister_code(KC_MNXT);
        }
      }
    case RGB_ANI:
      if(record->event.pressed){
        rgb_timer = timer_read();
      } else {
        if(timer_elapsed(rgb_timer) > 300){
          rgblight_mode(1);
        } else {
          return MACRO(T(RGB_MOD))
        }
      }
  }
  return MACRO_NONE;
};



bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case CF_EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      return false;
      break;
    case CF_VERS:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      return false;
      break;
  }
}

LEADER_EXTERNS();
void matrix_scan_user(void) {

  uint8_t layer = biton32(layer_state);

  if (keyboard_report->mods & MOD_BIT(KC_LSFT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LSFT)) && 
      !has_oneshot_mods_timed_out())) {
    ergogox_right_led_1_set(LED_BRIGHTNESS_HI);
    ergodox_right_led_1_on();
  } else if(layer == MDIA || layer == MOBA || layer == FPS || layer == CONF) {
    ergogox_right_led_1_set(LED_BRIGHTNESS_LO);
    ergodox_right_led_1_on();
  } else {
    ergodox_right_led_1_off();
  }

  if (keyboard_report->mods & MOD_BIT(KC_LCTL) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LCTL)) && 
      !has_oneshot_mods_timed_out())) {
    ergogox_right_led_2_set(LED_BRIGHTNESS_HI);
    ergodox_right_led_2_on();
  } else if(layer == NUMP || layer == FUNC || layer == FPS || layer == CONF) {
    ergogox_right_led_2_set(LED_BRIGHTNESS_LO);
    ergodox_right_led_2_on();
  } else {
    ergodox_right_led_2_off();
  }
  
  if (keyboard_report->mods & MOD_BIT(KC_LALT) ||
      ((get_oneshot_mods() & MOD_BIT(KC_LALT)) && 
      !has_oneshot_mods_timed_out())) {
    ergogox_right_led_3_set(LED_BRIGHTNESS_HI);
    ergodox_right_led_3_on();
  } else if(layer == TSYM || layer == FUNC || layer == MOBA || layer == CONF) {
    ergogox_right_led_3_set(LED_BRIGHTNESS_LO);
    ergodox_right_led_3_on();
  } else {
    ergodox_right_led_3_off();
  }

  LEADER_DICTIONARY() {
    leading = false;
    leader_end();

    SEQ_THREE_KEYS(KC_W, KC_I, KC_N) { os_type = OS_WIN; }
    SEQ_THREE_KEYS(KC_O, KC_S, KC_X) { os_type = OS_OSX; }
    SEQ_THREE_KEYS(KC_L, KC_I, KC_N) { os_type = OS_LIN; }

    SEQ_ONE_KEY(KC_A) {
      switch(os_type) {
        case OS_WIN: 
          MACRO( T(KC_NLCK), D(KC_RALT), T(KC_KP_0), T(KC_KP_2), T(KC_KP_2), T(KC_KP_8), U(KC_RALT), T(KC_NLCK) END );
          break;
        case OS_OSX:
          MACRO( D(KC_RALT), D(KC_COLN), U(KC_COLN), U(KC_RALT), T(KC_A) END )
          break;
        case OS_LIN:
          MACRO( T(KC_RALT), T(KC_DQT), T(KC_A) END )
          break;
      }
    }
    SEQ_TWO_KEYS(KC_A, KC_A) {
      switch(os_type) {
        case OS_WIN:
          MACRO( T(KC_NLCK), D(KC_RALT), T(KC_KP_0), T(KC_KP_1), T(KC_KP_9), T(KC_KP_6), U(KC_RALT), T(KC_NLCK) END );
          break;
        case OS_OSX: 
          MACRO( D(KC_RALT), D(KC_COLN), U(KC_COLN), U(KC_RALT), T(LSFT(KC_A)) END )
          break;
        case OS_LIN:
          MACRO( T(KC_RALT), T(KC_DQT), T(LSFT(KC_A)) END )
          break;
      }
    }
    SEQ_ONE_KEY(KC_O) {
      switch(os_type) {
        case OS_WIN:
          MACRO( T(KC_NLCK), D(KC_RALT), T(KC_KP_0), T(KC_KP_2), T(KC_KP_4), T(KC_KP_6), U(KC_RALT), T(KC_NLCK) END );
          break;
        case OS_OSX:
          MACRO( D(KC_RALT), D(KC_COLN), U(KC_COLN), U(KC_RALT), T(KC_O) END )
          break;
        case OS_LIN:
          MACRO( T(KC_RALT), T(KC_DQT), T(KC_O) END )
          break;
      }
    }
    SEQ_TWO_KEYS(KC_O, KC_O) {
      switch(os_type) {
        case OS_WIN:
          MACRO( T(KC_NLCK), D(KC_RALT), T(KC_KP_0), T(KC_KP_2), T(KC_KP_1), T(KC_KP_4), U(KC_RALT), T(KC_NLCK) END );
          break;
        case OS_OSX:
          MACRO( D(KC_RALT), D(KC_COLN), U(KC_COLN), U(KC_RALT), T(LSFT(KC_O)) END )
          break;
        case OS_LIN:
          MACRO( T(KC_RALT), T(KC_DQT), T(LSFT(KC_O)) END )
          break;
      }
    }
    SEQ_ONE_KEY(KC_U) {
      switch(os_type) {
        case OS_WIN:
          MACRO( T(KC_NLCK), D(KC_RALT), T(KC_KP_0), T(KC_KP_2), T(KC_KP_5), T(KC_KP_2), U(KC_RALT), T(KC_NLCK) END );
          break;
        case OS_OSX: 
          MACRO( D(KC_RALT), D(KC_COLN), U(KC_COLN), U(KC_RALT), T(KC_U) END )
          break;
        case OS_LIN:
          MACRO( T(KC_RALT), T(KC_DQT), T(KC_U) END )
          break;
      }
    }
    SEQ_TWO_KEYS(KC_U, KC_U) {
      switch(os_type) {
        case OS_WIN:
          MACRO( T(KC_NLCK), D(KC_RALT), T(KC_KP_0), T(KC_KP_2), T(KC_KP_2), T(KC_KP_0), U(KC_RALT), T(KC_NLCK) END );
          break;
        case OS_OSX:
          MACRO( D(KC_RALT), D(KC_COLN), U(KC_COLN), U(KC_RALT), T(LSFT(KC_U)) END )
          break;
        case OS_LIN: 
          MACRO( T(KC_RALT), T(KC_DQT), T(LSFT(KC_U)) END )
          break;
      }
    }
    SEQ_ONE_KEY(KC_S) {
      switch(os_type) {
        case OS_WIN:
          MACRO( T(KC_NLCK), D(KC_RALT), T(KC_KP_0), T(KC_KP_2), T(KC_KP_2), T(KC_KP_3), U(KC_RALT), T(KC_NLCK) END );
          break;
        case OS_OSX: 
          MACRO( D(KC_RALT), T(KC_S) U(KC_RALT) END )
          break;
        case OS_LIN:
          MACRO( T(KC_RALT), T(KC_S), T(KC_S) END )
          break;
      }
    }
  }

}

void matrix_init_user(void) {

  ergodox_led_all_on();
  rgblight_enable();
  rgblight_setrgb(255, 0, 0);
  for (int i = LED_BRIGHTNESS_HI; i > LED_BRIGHTNESS_LO; i--) {
    ergodox_led_all_set (i);
    wait_ms (5);
  }
  rgblight_setrgb(255,255,0);
  wait_ms(1000);
  for (int i = LED_BRIGHTNESS_LO; i > 0; i--) {
    ergodox_led_all_set (i);
    wait_ms (10);
  }
  rgblight_setrgb(0,255,255);
  ergodox_led_all_off();
  wait(1000);
  
  rgblight_effect_knight(50);
}
