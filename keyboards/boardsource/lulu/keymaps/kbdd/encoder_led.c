#include QMK_KEYBOARD_H
#include "encoder_led.h"
#include "transactions.h"

static bool g_encoder_led_sync_initialized = false;
static bool g_encoder_clockwise            = false;

static bool g_encoder_led_sync_split_initialized = false;

void encoder_led_sync_init(void) {
    g_encoder_led_sync_initialized = true;
    g_encoder_clockwise            = false;
}

static void encoder_led_sync_slave_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    const bool clockwise = *(const bool*)in_data;
    g_encoder_clockwise  = clockwise;
}

void encoder_led_sync_init_split_sync(void) {
    if (g_encoder_led_sync_split_initialized || !g_encoder_led_sync_initialized) return;

    // Register the slave-side handler for WPM sync
    transaction_register_rpc(ENCODER_LED_SYNC, encoder_led_sync_slave_handler);
    g_encoder_led_sync_split_initialized = true;
}

void encoder_led_sync_on_keyevent(keyrecord_t *record) {
    if (!g_encoder_led_sync_initialized) return;

    if (is_keyboard_master()) {
        bool state_changed = false;
        if (record->event.type == ENCODER_CW_EVENT) {
            if (!g_encoder_clockwise) {
                g_encoder_clockwise = true;
                state_changed = true;
            }
        } else if (record->event.type == ENCODER_CCW_EVENT) {
            if (g_encoder_clockwise) {
                g_encoder_clockwise = false;
                state_changed = true;
            }
        }

        // Immediately sync to slave when direction changes
        if (state_changed) {
            transaction_rpc_send(ENCODER_LED_SYNC, sizeof(bool), &g_encoder_clockwise);
        }
    }
}

void encoder_led_sync_rgb_task(void) {
    if (!g_encoder_led_sync_initialized) return;

    if (!is_keyboard_master()) {
        if (last_encoder_activity_elapsed() < 500) {
            if (g_encoder_clockwise) {
                rgb_matrix_set_color(ENCODER_LED_INDEX, 0, 0xff, 0);
            } else {
                rgb_matrix_set_color(ENCODER_LED_INDEX, 0xff, 0, 0);
            }
        }
    }
}

bool encoder_led_sync_get(encoder_led_t *sync) {
    if (!sync || !g_encoder_led_sync_initialized) {
        return false;
    }

    sync->clockwise = g_encoder_clockwise;

    return true;
}

void encoder_led_sync_housekeeping_task(void) {
    if (!g_encoder_led_sync_initialized) return;

    if (is_keyboard_master()) {
        // Sync with slave every 250ms
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 250) {
            if (transaction_rpc_send(ENCODER_LED_SYNC, sizeof(bool), &g_encoder_clockwise)) {
                last_sync = timer_read32();
            } else {
                // Sync failed, will retry next time
            }
        }
    }
}
