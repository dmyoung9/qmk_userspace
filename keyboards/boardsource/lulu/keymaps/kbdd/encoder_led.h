#include QMK_KEYBOARD_H

#define ENCODER_LED_INDEX 65

void encoder_led_sync_init_split_sync(void);
void encoder_led_sync_on_keyevent(keyrecord_t *record);
void encoder_led_sync_rgb_task(void);
