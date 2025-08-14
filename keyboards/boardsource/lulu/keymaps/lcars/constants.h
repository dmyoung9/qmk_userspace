enum layers {
    _QWERTY,
	_LOWER,
    _RAISE,
    _ADJUST
};

// simple layers, no tri-layer
#define LOWER MO(_LOWER)
#define RAISE MO(_RAISE)
#define ADJUST MO(_ADJUST)

// mouse macros to keep layout consistent
#define KC_MB1 KC_MS_BTN1
#define KC_MB2 KC_MS_BTN2
#define KC_MUP KC_MS_UP
#define KC_MDWN KC_MS_DOWN
#define KC_MLFT KC_MS_LEFT
#define KC_MRGT KC_MS_RIGHT