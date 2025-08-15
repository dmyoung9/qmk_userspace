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
    SLICE_CUSTOM(layer_0_0, 72, 2),
    SLICE_CUSTOM(layer_0_1, 72, 2),
    SLICE_CUSTOM(layer_0_2, 72, 2),
    SLICE_CUSTOM(layer_0_3, 72, 2),
    SLICE_CUSTOM(layer_0_4, 72, 2),
    SLICE_CUSTOM(layer_0_5, 72, 2)
);
DEFINE_SLICE_SEQ(layer_1,
    SLICE_CUSTOM(layer_1_0, 72, 2),
    SLICE_CUSTOM(layer_1_1, 72, 2),
    SLICE_CUSTOM(layer_1_2, 72, 2),
    SLICE_CUSTOM(layer_1_3, 72, 2),
    SLICE_CUSTOM(layer_1_4, 72, 2),
    SLICE_CUSTOM(layer_1_5, 72, 2)
);
DEFINE_SLICE_SEQ(layer_2,
    SLICE_CUSTOM(layer_2_0, 72, 2),
    SLICE_CUSTOM(layer_2_1, 72, 2),
    SLICE_CUSTOM(layer_2_2, 72, 2),
    SLICE_CUSTOM(layer_2_3, 72, 2),
    SLICE_CUSTOM(layer_2_4, 72, 2),
    SLICE_CUSTOM(layer_2_5, 72, 2),
    SLICE_CUSTOM(layer_2_6, 72, 2),
    SLICE_CUSTOM(layer_2_7, 72, 2),
    SLICE_CUSTOM(layer_2_8, 72, 2),
    SLICE_CUSTOM(layer_2_9, 72, 2)
);
DEFINE_SLICE_SEQ(layer_3,
    SLICE_CUSTOM(layer_3_0, 72, 2),
    SLICE_CUSTOM(layer_3_1, 72, 2),
    SLICE_CUSTOM(layer_3_2, 72, 2),
    SLICE_CUSTOM(layer_3_3, 72, 2),
    SLICE_CUSTOM(layer_3_4, 72, 2),
    SLICE_CUSTOM(layer_3_5, 72, 2),
    SLICE_CUSTOM(layer_3_6, 72, 2),
    SLICE_CUSTOM(layer_3_7, 72, 2)
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
    .x = 56, .y = 0,           // Position
    .bbox_w = 72, .bbox_h = 12, // Bounding box
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

// Layer frame animation controller (using boot-then-reverse-out-back pattern)
bootrev_anim_t layer_frame_anim;
static uint8_t last_layer = 0;

static const slice_t SLICE_wpm_frame = SLICE128x32(wpm_frame);
static const slice_t SLICE_logo = SLICE22x16(logo);

void init_widgets(void) {
    uint32_t now = timer_read32();
    widget_init(&layer_widget, &layer_widget_config, 0, now);

    // Initialize layer frame animation with boot animation
    // Boot: 0→4, stay at 4. Layer changes: 4→0→4
    bootrev_anim_init(&layer_frame_anim, &layer_frame, 42, 0, /*run_boot_anim=*/true, now);

    last_layer = get_highest_layer(layer_state);  // Initialize current layer
}

void tick_widgets(void) {
    uint32_t now = timer_read32();

    // Check for layer changes and trigger animation
    uint8_t current_layer = get_highest_layer(layer_state);
    if (bootrev_anim_boot_done(&layer_frame_anim) && current_layer != last_layer) {
        last_layer = current_layer;
        bootrev_anim_trigger(&layer_frame_anim, now);
    }

    // Render the layer frame animation
    bootrev_anim_render(&layer_frame_anim, now);

    widget_tick(&layer_widget, now);
}

void draw_wpm_frame(void) {
    draw_slice_px(&SLICE_wpm_frame, 0, 0);
}

void draw_logo(void) {
    draw_slice_px(&SLICE_logo, 106, 16);
}
