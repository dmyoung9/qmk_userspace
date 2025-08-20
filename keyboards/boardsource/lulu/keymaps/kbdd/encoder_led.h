#include QMK_KEYBOARD_H

typedef struct {
    bool clockwise;
} encoder_led_t;

#define ENCODER_LED_INDEX 65

void encoder_led_sync_init(void);
void encoder_led_sync_init_split_sync(void);
void encoder_led_sync_on_keyevent(keyrecord_t *record);
void encoder_led_sync_rgb_task(void);
void encoder_led_sync_housekeeping_task(void);
