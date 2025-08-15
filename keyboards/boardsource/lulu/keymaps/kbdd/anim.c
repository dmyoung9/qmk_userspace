#include QMK_KEYBOARD_H
#include "anim.h"
#include "progmem_anim.h"
#include "oled_utils.h"
#include "oled_declarative.h"


#define SLICE22x16(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 22, 2})
#define SLICE32x16(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 32, 2})
#define SLICE128x32(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 128, 4})
#define SLICE106x16(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 106, 2})
#define SLICE86x16(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 86, 2})
#define SLICE41x16(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 41, 2})

DEFINE_SLICE_SEQ(layer_0,
    SLICE32x16(layer_0_0),
    SLICE32x16(layer_0_1),
    SLICE32x16(layer_0_2),
    SLICE32x16(layer_0_3)
);
DEFINE_SLICE_SEQ(layer_1,
    SLICE32x16(layer_1_0),
    SLICE32x16(layer_1_1),
    SLICE32x16(layer_1_2),
    SLICE32x16(layer_1_3)
);
DEFINE_SLICE_SEQ(layer_2,
    SLICE32x16(layer_2_0),
    SLICE32x16(layer_2_1),
    SLICE32x16(layer_2_2),
    SLICE32x16(layer_2_3)
);
DEFINE_SLICE_SEQ(layer_3,
    SLICE32x16(layer_3_0),
    SLICE32x16(layer_3_1),
    SLICE32x16(layer_3_2),
    SLICE32x16(layer_3_3)
);

static const state_desc_t layer_states[] = {
    STATE_FWD(&layer_0),  // Layer 0: forward animation
    STATE_FWD(&layer_1),  // Layer 1: forward animation
    STATE_FWD(&layer_2),  // Layer 2: forward animation
    STATE_FWD(&layer_3)   // Layer 3: forward animation
};

static uint8_t layer_query(uint32_t user_arg) {
    return get_highest_layer(layer_state);
}

const widget_config_t layer_widget_config = {
    .x = 0, .y = 0,           // Position
    .bbox_w = 32, .bbox_h = 16, // Bounding box
    .blit = BLIT_OPAQUE,       // Clear background each frame
    .states = layer_states,
    .state_count = LAYER_COUNT,
    .query = layer_query,
    .user_arg = 0,
    .initial_state = 0
};

// Runtime instance
widget_t layer_widget;

DEFINE_SLICE_SEQ(layer_frame,
    SLICE86x16(layer_frame_0),
    SLICE86x16(layer_frame_1),
    SLICE86x16(layer_frame_2),
    SLICE86x16(layer_frame_3),
    SLICE86x16(layer_frame_4)
);

static const state_desc_t layer_frame_states[] = {
    STATE_FWD(&layer_frame)
};

static uint8_t layer_frame_query(uint32_t user_arg) {
    return 0;
}

const widget_config_t layer_frame_widget_config = {
    .x = 42, .y = 0,           // Position
    .bbox_w = 86, .bbox_h = 16, // Bounding box
    .blit = BLIT_OPAQUE,       // Clear background each frame
    .states = layer_frame_states,
    .state_count = 1,
    .query = layer_frame_query,
    .user_arg = 0,
    .initial_state = 0
};

// Runtime instance
widget_t layer_frame_widget;

static const slice_t SLICE_wpm_frame = SLICE128x32(wpm_frame);
static const slice_t SLICE_logo = SLICE22x16(logo);

void init_widgets(void) {
    widget_init(&layer_widget, &layer_widget_config, 0, timer_read32());
    widget_init(&layer_frame_widget, &layer_frame_widget_config, 0, timer_read32());
}

void tick_widgets(void) {
    widget_tick(&layer_widget, timer_read32());
    widget_tick(&layer_frame_widget, timer_read32());
}

void draw_wpm_frame(void) {
    draw_slice_px(&SLICE_wpm_frame, 0, 0);
}

void draw_logo(void) {
    draw_slice_px(&SLICE_logo, 106, 16);
}
