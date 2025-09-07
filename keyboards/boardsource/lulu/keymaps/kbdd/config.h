#define ENABLE_RGB_MATRIX_TYPING_HEATMAP
#define RGB_MATRIX_DEFAULT_MODE RGB_MATRIX_TYPING_HEATMAP

#define RGB_MATRIX_SLEEP

// RGB Fade-out configuration
// #define RGB_FADE_START_TIMEOUT 14000  // Start fading after 10 seconds of inactivity
// #define RGB_FADE_STEP_INTERVAL 60    // Fade step every 500ms (uses RGB_MATRIX_VAL_STEP for step size)
// #define RGB_FADE_MIN_BRIGHTNESS 0    // Minimum brightness before turning off completely
// #define RGB_FADE_MIN_SATURATION 0    // Minimum saturation before turning off completely

// RGB Fade-in configuration
// #define RGB_FADE_IN_STEP_INTERVAL 100 // Fade-in step every 100ms (faster than fade-out for responsiveness)

#define OLED_TIMEOUT 15000

#define ENCODER_LED_INDEX 65
#ifdef ENCODER_ENABLE
#    undef ENCODER_A_PINS
#    undef ENCODER_B_PINS
#    undef ENCODER_RESOLUTIONS
#    undef ENCODER_A_PINS_RIGHT
#    undef ENCODER_B_PINS_RIGHT
#    undef ENCODER_RESOLUTIONS_RIGHT

#    define ENCODER_A_PINS { }
#    define ENCODER_B_PINS { }
#    define ENCODER_RESOLUTIONS { }
#    define ENCODER_A_PINS_RIGHT {F5}
#    define ENCODER_B_PINS_RIGHT {F4}
#    define ENCODER_RESOLUTIONS_RIGHT {4}
#endif

#define SPLIT_LAYER_STATE_ENABLE

// Faster animation frame rate to reduce chance of getting stuck during rapid layer changes
// Default is 80ms, reducing to 50ms makes transitions more responsive
#define ANIM_FRAME_MS 80

// Widget watchdog configuration
// Timeout before considering an animation stuck (default: 1000ms)
#define WIDGET_WATCHDOG_TIMEOUT_MS 1000
// Grace period before forcing reset (default: 500ms)
#define WIDGET_WATCHDOG_GRACE_MS 500

// Enable debugging for animation issues (optional - can be commented out for production)
// #define OLED_ANIM_DEBUG

// WPM Bar Graph Configuration for 128x32 SSD1306
#define WPM_BAR_X 0
#define WPM_BAR_Y 13
#define WPM_BAR_WIDTH 78
#define WPM_BAR_HEIGHT 11

#define SPLIT_WPM_ENABLE
#define SPLIT_ACTIVITY_ENABLE

#undef SPLIT_TRANSACTION_IDS_USER
#define SPLIT_TRANSACTION_IDS_USER WPM_STATS_SYNC, ENCODER_LED_SYNC

#define CAPS_WORD_INVERT_ON_SHIFT

// LUMINO
#define LUMINO_HIGH_BRIGHTNESS 0.8
#define LUMINO_LOW_BRIGHTNESS 0.4

#define LUMINO_LONG_TIMEOUT 60000
#define LUMINO_SOON_TIMEOUT 15000
#define LUMINO_TRANSITION 500

#define LUMINO_BOOT_COLOR RGB_RED
