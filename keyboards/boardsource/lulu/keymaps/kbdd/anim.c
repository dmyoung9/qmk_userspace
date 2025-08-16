#include QMK_KEYBOARD_H
#include "anim.h"
#include "progmem_anim.h"
#include "oled_utils.h"
#include "oled_anim.h"
#include "oled_declarative.h"

#define LAYER_COUNT 4



#define SLICE72x12(p) SLICE_CUSTOM_PX(p, 72, 12)
#define SLICE22x16(p) SLICE_CUSTOM_PX(p, 22, 16)
#define SLICE32x16(p) SLICE_CUSTOM_PX(p, 32, 16)
#define SLICE128x32(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 128, 4, 0})
#define SLICE106x16(p) SLICE_CUSTOM_PX(p, 106, 16)
#define SLICE86x16(p) SLICE_CUSTOM_PX(p, 86, 16)
#define SLICE41x16(p) SLICE_CUSTOM_PX(p, 41, 16)

// Modifier slice macros (9px high)
#define SLICE21x9(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 21, 2, 9})
#define SLICE25x9(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 25, 2, 9})
#define SLICE17x9(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 17, 2, 9})
#define SLICE23x9(p) ((const slice_t){(const uint8_t PROGMEM *)(p), 23, 2, 9})

DEFINE_SLICE_SEQ(layer_0,
    SLICE72x12(layer_0_0),
    SLICE72x12(layer_0_1),
    SLICE72x12(layer_0_2),
    SLICE72x12(layer_0_3),
    SLICE72x12(layer_0_4),
    SLICE72x12(layer_0_5)
);
DEFINE_SLICE_SEQ(layer_1,
    SLICE72x12(layer_1_0),
    SLICE72x12(layer_1_1),
    SLICE72x12(layer_1_2),
    SLICE72x12(layer_1_3),
    SLICE72x12(layer_1_4),
    SLICE72x12(layer_1_5)
);
DEFINE_SLICE_SEQ(layer_2,
    SLICE72x12(layer_2_0),
    SLICE72x12(layer_2_1),
    SLICE72x12(layer_2_2),
    SLICE72x12(layer_2_3),
    SLICE72x12(layer_2_4),
    SLICE72x12(layer_2_5),
    SLICE72x12(layer_2_6),
    SLICE72x12(layer_2_7),
    SLICE72x12(layer_2_8),
    SLICE72x12(layer_2_9)
);
DEFINE_SLICE_SEQ(layer_3,
    SLICE72x12(layer_3_0),
    SLICE72x12(layer_3_1),
    SLICE72x12(layer_3_2),
    SLICE72x12(layer_3_3),
    SLICE72x12(layer_3_4),
    SLICE72x12(layer_3_5),
    SLICE72x12(layer_3_6),
    SLICE72x12(layer_3_7)
);

// Modifier animation sequences
// TODO: Temporarily disabled to restore keyboard functionality
/*
DEFINE_SLICE_SEQ(caps_seq,
    SLICE21x9(caps_0),
    SLICE21x9(caps_1),
    SLICE21x9(caps_2),
    SLICE21x9(caps_3)
);

DEFINE_SLICE_SEQ(super_seq,
    SLICE25x9(super_0),
    SLICE25x9(super_1),
    SLICE25x9(super_2),
    SLICE25x9(super_3)
);

DEFINE_SLICE_SEQ(alt_seq,
    SLICE17x9(alt_0),
    SLICE17x9(alt_1),
    SLICE17x9(alt_2),
    SLICE17x9(alt_3)
);

DEFINE_SLICE_SEQ(shift_seq,
    SLICE23x9(shift_0),
    SLICE23x9(shift_1),
    SLICE23x9(shift_2),
    SLICE23x9(shift_3)
);

DEFINE_SLICE_SEQ(ctrl_seq,
    SLICE21x9(ctrl_0),
    SLICE21x9(ctrl_1),
    SLICE21x9(ctrl_2),
    SLICE21x9(ctrl_3)
);
*/

// Layer animation controllers (using oneshot pattern for proper enter/exit)
oneshot_anim_t layer_0_anim;
oneshot_anim_t layer_1_anim;
oneshot_anim_t layer_2_anim;
oneshot_anim_t layer_3_anim;

// Array for easy access
static oneshot_anim_t* layer_anims[] = {
    &layer_0_anim,
    &layer_1_anim,
    &layer_2_anim,
    &layer_3_anim
};

// Array of sequences for easy access
static const slice_seq_t* layer_sequences[] = {
    &layer_0,
    &layer_1,
    &layer_2,
    &layer_3
};

// Track layer states for proper enter/exit behavior
static bool layer_is_active[LAYER_COUNT] = {false, false, false, false};

// Layer transition state machine
typedef enum {
    LAYER_TRANSITION_IDLE = 0,
    LAYER_TRANSITION_EXITING,
    LAYER_TRANSITION_ENTERING
} layer_transition_state_t;

static layer_transition_state_t transition_state = LAYER_TRANSITION_IDLE;
static uint8_t exiting_layer = 0;
static uint8_t entering_layer = 0;

// Modifier state detection functions
// TODO: Temporarily disabled to restore keyboard functionality
/*
static bool is_caps_active(void) {
    return host_keyboard_led_state().caps_lock || is_caps_word_on();
}

static bool is_super_active(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    return (mods & MOD_MASK_GUI) != 0;
}

static bool is_alt_active(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    return (mods & MOD_MASK_ALT) != 0;
}

static bool is_shift_active(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    return (mods & MOD_MASK_SHIFT) != 0;
}

static bool is_ctrl_active(void) {
    uint8_t mods = get_mods() | get_oneshot_mods();
    return (mods & MOD_MASK_CTRL) != 0;
}
*/

// Custom layer animation functions
static void trigger_layer_exit(uint8_t layer, uint32_t now) {
    if (layer >= LAYER_COUNT) return;
    // Start reverse animation (last frame -> first frame)
    animator_start(&layer_anims[layer]->anim, layer_sequences[layer], /*forward=*/false, now);
    layer_anims[layer]->phase = ONESHOT_TRIGGERED;
    layer_is_active[layer] = false;
}

static void trigger_layer_enter(uint8_t layer, uint32_t now) {
    if (layer >= LAYER_COUNT) return;
    // Start forward animation (first frame -> last frame)
    animator_start(&layer_anims[layer]->anim, layer_sequences[layer], /*forward=*/true, now);
    layer_anims[layer]->phase = ONESHOT_TRIGGERED;
    layer_is_active[layer] = true;
}

static void render_layer_anim(oneshot_anim_t *w, bool is_active, uint32_t now) {
    switch (w->phase) {
        case ONESHOT_IDLE:
            // Draw steady frame based on active state
            if (w->seq && w->seq->count) {
                const slice_t *steady = is_active
                    ? &w->seq->frames[w->seq->count - 1]  // Last frame (active)
                    : &w->seq->frames[0];                 // First frame (inactive)
                draw_slice_px(steady, w->x, w->y);
            }
            break;

        case ONESHOT_BOOT:
        case ONESHOT_TRIGGERED: {
            anim_result_t r = animator_step_and_draw_blend(&w->anim, w->x, w->y, now);
            if (r == ANIM_DONE_AT_END || r == ANIM_DONE_AT_START) {
                // Animation completed
                w->phase = ONESHOT_IDLE;
                w->boot_done = true;
            }
            break;
        }
    }
}



static uint8_t current_layer = 0;

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

// Modifier animation controllers (using toggle pattern)
// TODO: Temporarily disabled to restore keyboard functionality
/*
toggle_anim_t caps_anim;
toggle_anim_t super_anim;
toggle_anim_t alt_anim;
toggle_anim_t shift_anim;
toggle_anim_t ctrl_anim;
*/

static const slice_t SLICE_wpm_frame = SLICE128x32(wpm_frame);
static const slice_t SLICE_logo = SLICE22x16(logo);

void init_widgets(void) {
    uint32_t now = timer_read32();

    // Initialize all layer animations with boot animation
    // Only layer 0 runs boot animation initially, others start idle
    current_layer = get_highest_layer(layer_state);

    for (uint8_t i = 0; i < LAYER_COUNT; i++) {
        bool is_active = (i == current_layer);
        bool run_boot = is_active;  // Only current layer runs boot animation
        // steady_at_end=true means idle state shows last frame (active state)
        oneshot_anim_init(layer_anims[i], layer_sequences[i], 56, 0, /*steady_at_end=*/true, run_boot, now);
        layer_is_active[i] = is_active;
    }

    // Initialize layer frame animation with boot animation
    // Boot: 0→4, stay at 4. Layer changes: 4→0→4
    bootrev_anim_init(&layer_frame_anim, &layer_frame, 42, 0, /*run_boot_anim=*/true, now);

    // Initialize modifier animations (all start at frame 0, inactive)
    // TODO: Temporarily disabled to restore keyboard functionality
    // toggle_anim_init(&caps_anim, &caps_seq, 10, 2, false, now);
    // toggle_anim_init(&super_anim, &super_seq, 9, 22, false, now);
    // toggle_anim_init(&alt_anim, &alt_seq, 35, 22, false, now);
    // toggle_anim_init(&shift_anim, &shift_seq, 53, 22, false, now);
    // toggle_anim_init(&ctrl_anim, &ctrl_seq, 77, 22, false, now);

    last_layer = get_highest_layer(layer_state);  // Initialize current layer
}

void tick_widgets(void) {
    uint32_t now = timer_read32();

    // Handle layer transition state machine with cancellation support
    uint8_t new_layer = get_highest_layer(layer_state);

    switch (transition_state) {
        case LAYER_TRANSITION_IDLE:
            // Check for layer changes
            if (new_layer != current_layer) {
                // Start exit animation on old layer
                if (oneshot_anim_boot_done(layer_anims[current_layer])) {
                    trigger_layer_exit(current_layer, now);
                    transition_state = LAYER_TRANSITION_EXITING;
                    exiting_layer = current_layer;
                    entering_layer = new_layer;

                    // Trigger layer frame animation
                    if (bootrev_anim_boot_done(&layer_frame_anim)) {
                        bootrev_anim_trigger(&layer_frame_anim, now);
                    }
                }
            }
            break;

        case LAYER_TRANSITION_EXITING:
            // Check for layer change during exit (cancellation)
            if (new_layer != entering_layer) {
                // Layer changed again! Update the target but let exit finish
                entering_layer = new_layer;
            }

            // Wait for exit animation to complete
            if (!oneshot_anim_is_running(layer_anims[exiting_layer])) {
                // Exit completed, start enter animation for the final target layer
                if (oneshot_anim_boot_done(layer_anims[entering_layer])) {
                    trigger_layer_enter(entering_layer, now);
                    transition_state = LAYER_TRANSITION_ENTERING;
                    current_layer = entering_layer;
                    last_layer = entering_layer;
                }
            }
            break;

        case LAYER_TRANSITION_ENTERING:
            // Check for layer change during enter (cancellation)
            if (new_layer != entering_layer) {
                // Layer changed during enter! Start new exit sequence
                if (oneshot_anim_boot_done(layer_anims[entering_layer])) {
                    trigger_layer_exit(entering_layer, now);
                    transition_state = LAYER_TRANSITION_EXITING;
                    exiting_layer = entering_layer;
                    entering_layer = new_layer;

                    // Trigger layer frame animation
                    if (bootrev_anim_boot_done(&layer_frame_anim)) {
                        bootrev_anim_trigger(&layer_frame_anim, now);
                    }
                }
            } else {
                // Wait for enter animation to complete
                if (!oneshot_anim_is_running(layer_anims[entering_layer])) {
                    // Enter completed, transition finished
                    transition_state = LAYER_TRANSITION_IDLE;
                }
            }
            break;
    }


    // Render the layer frame animation FIRST (background) - opaque
    bootrev_anim_render(&layer_frame_anim, now);

    // Render all layer animations SECOND (foreground) - OR blend to not overwrite frame
    for (uint8_t i = 0; i < LAYER_COUNT; i++) {
        bool should_render = false;

        switch (transition_state) {
            case LAYER_TRANSITION_IDLE:
                // Only render current layer
                should_render = (i == current_layer);
                break;

            case LAYER_TRANSITION_EXITING:
                // Only render exiting layer
                should_render = (i == exiting_layer);
                break;

            case LAYER_TRANSITION_ENTERING:
                // Only render entering layer
                should_render = (i == entering_layer);
                break;
        }

        if (should_render) {
            render_layer_anim(layer_anims[i], layer_is_active[i], now);
        }
    }

    // Update modifier animations based on current state
    // TODO: Temporarily disabled to restore keyboard functionality
    // toggle_anim_set(&caps_anim, is_caps_active(), now);
    // toggle_anim_set(&super_anim, is_super_active(), now);
    // toggle_anim_set(&alt_anim, is_alt_active(), now);
    // toggle_anim_set(&shift_anim, is_shift_active(), now);
    // toggle_anim_set(&ctrl_anim, is_ctrl_active(), now);

    // Render modifier animations
    // TODO: Temporarily disabled to restore keyboard functionality
    // toggle_anim_render(&caps_anim, now);
    // toggle_anim_render(&super_anim, now);
    // toggle_anim_render(&alt_anim, now);
    // toggle_anim_render(&shift_anim, now);
    // toggle_anim_render(&ctrl_anim, now);
}

void draw_wpm_frame(void) {
    draw_slice_px(&SLICE_wpm_frame, 0, 0);
}

void draw_logo(void) {
    draw_slice_px(&SLICE_logo, 106, 16);
}
