#include QMK_KEYBOARD_H
#include "encoder_led.h"
#include "transactions.h"

static bool     g_encoder_led_sync_initialized  = false;
static uint32_t g_last_encoder_activity_elapsed = 0;
static bool     g_encoder_clockwise             = false;

static encoder_led_t g_slave_encoder_led_data = {0, false};

static bool g_encoder_led_sync_split_initialized = false;

void encoder_led_sync_init(void) {
    g_encoder_led_sync_initialized  = true;
    g_last_encoder_activity_elapsed = last_encoder_activity_elapsed();
    g_encoder_clockwise             = false;
}

static void encoder_led_sync_slave_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
    const encoder_led_t *master_data = (const encoder_led_t *)in_data;
    g_slave_encoder_led_data         = *master_data;
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
        bool encoder_clockwise;

        if (record->event.type == ENCODER_CW_EVENT) {
            encoder_clockwise = true;
        } else if (record->event.type == ENCODER_CCW_EVENT) {
            encoder_clockwise = false;
        }

        g_encoder_clockwise = encoder_clockwise;
    }
}

void encoder_led_sync_rgb_task(void) {
    if (!g_encoder_led_sync_initialized) return;

    if (!is_keyboard_master()) {
        if (g_slave_encoder_led_data.elapsed < 500) {
            if (g_slave_encoder_led_data.clockwise) {
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
    sync->elapsed   = last_encoder_activity_elapsed();

    return true;
}

void encoder_led_sync_housekeeping_task(void) {
    if (!g_encoder_led_sync_initialized) return;

    if (is_keyboard_master()) {
        // Sync with slave every 250ms
        static uint32_t last_sync = 0;
        if (timer_elapsed32(last_sync) > 250) {
            // Update the elapsed time in the master data before syncing
            encoder_led_t m2s;
            encoder_led_sync_get(&m2s);

            if (transaction_rpc_send(ENCODER_LED_SYNC, sizeof(m2s), &m2s)) {
                last_sync = timer_read32();
            } else {
                // Sync failed, will retry next time
            }
        }
    }
}
