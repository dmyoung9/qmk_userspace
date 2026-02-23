# OLED Animation Refactor Plan (KBDD + oled_utils)

## Status Refresh (2026-02-21)
- Overall: **Complete for planned scope**. Core API/behavior gaps are closed and docs are aligned with shipped behavior.
- Complete phases: **Phase 1, Phase 3, Phase 4, Phase 5**
- Partially complete phases: **Phase 2**
- Pending phases: **none**

### Current blockers
- Optional cleanup: align keymap-local layer count usage with shared constants to avoid duplication.

## Decision
- Layer transition behavior: **Option 1**
- Semantics: skip intermediate layers during rapid changes, always converge to latest requested layer.

## Goals
- Keep current visual behavior and responsiveness in `kbdd`.
- Move reusable transition/controller logic into `modules/dmyoung9/oled_utils`.
- Define per-layer animation mapping from keymap data (enum-indexed table), similar to `unicode_map`.
- Bring module API/docs back in sync with shipped behavior.

## Phase 1: API Correctness in `oled_utils` (pre-migration)
- Implement missing public `widget_*` functions declared in `oled_declarative.h`:
  - `widget_validate_config`
  - `widget_force_state`
  - `widget_reset`
  - `widget_get_error`
- Fix oneshot boot completion bug in `oled_unified_anim.c` (ensure `boot_done` is set correctly after boot phase completes).
- Resolve `ANIM_LAYER_TRANSITION` contract mismatch:
  - Either implement behavior in unified controller, or
  - remove/deprecate public enum/macro/docs until implemented.
- Acceptance:
  - No declared-but-missing symbols.
  - Behavior matches header contract.

### Status
- **Complete**.
- Delivered:
  - Implemented all declared `widget_*` APIs in `oled_declarative.c`.
  - Fixed unified oneshot boot completion handling.
  - Implemented `ANIM_LAYER_TRANSITION` semantics in unified controller and aligned docs/header contract.

## Phase 2: Keymap-Defined Layer Animation Map
- Add a layer count constant aligned with `enum layers` in `constants.h`.
- Add a map type for layer animation assignment (descriptor/config pointer, not runtime instance).
- Add keymap map table (enum-indexed) in keymap-owned code.
- Initial shape:
  - `const unified_layer_anim_t layer_anim_map[LAYER_COUNT] = { ... }`
- Note on storage:
  - Start with plain `const` for simplicity/portability.
  - Only move to `PROGMEM` if flash pressure justifies AVR-specific pointer reads.
- Acceptance:
  - No hardcoded layer-to-animation ordering arrays in keymap logic.
  - Map is single source of truth for layer animation assignment.

### Status
- **Partially complete**.
- Current state:
  - Keymap now uses a single source-of-truth layer sequence map for transition assignment.
- Remaining work:
  - Optional: standardize on one layer-count source between keymap constants and animation module usage.

## Phase 3: Extract Transition Orchestration into `oled_utils`
- Introduce a reusable layer transition controller in module code:
  - runtime animation instances
  - transition state (`IDLE`, `EXITING`, `ENTERING`)
  - latest-target convergence (Option 1 semantics)
  - optional frame effect trigger integration
- Add callback-based hooks for user logic:
  - active layer query
  - optional gating/trigger callback for transition effects
  - optional pre/post render hooks
- Keep assets and layout decisions in keymap; move control flow/state machine to module.
- Acceptance:
  - `kbdd` no longer owns bespoke transition state machine logic.
  - Rapid layer switching still converges correctly to latest target.

### Status
- **Complete**.
- Current state:
  - Unified controller implements `ANIM_LAYER_TRANSITION` with Option 1 convergence semantics.
  - Keymap no longer owns bespoke transition orchestration logic.

## Phase 4: Migrate `kbdd/anim.c` to Module Controller
- Replace bespoke layer animation state machine with module controller API.
- Wire map-driven initialization and render/tick paths.
- Preserve rendering order and blend behavior:
  - background/frame elements before layer animation
  - modifiers after layer content
- Maintain existing function entrypoints used by keymap:
  - `init_widgets`
  - `tick_widgets`
  - `draw_wpm_frame`
  - `draw_logo`
- Acceptance:
  - Functional parity with current behavior.
  - No regression in transition convergence.

### Status
- **Complete**.
- Current state:
  - `kbdd` uses module controller API for layer transitions and preserves render order/entrypoints.

## Phase 5: Cleanup + Documentation Sync
- Fix WPM numeric narrowing issue in `kbdd` (`uint16_t` -> `uint8_t` wrap risk above 255).
- Update `oled_utils` docs/examples to match actual implementation and available APIs.
- Remove/mark examples that rely on non-implemented features.
- Acceptance:
  - Docs compile conceptually with current API.
  - No stale claims about unsupported behaviors.

### Status
- **Complete**.
- Remaining work:
  - None for planned scope.

## Prioritized Execution Checklist

### Priority 0 (quick correctness and contract safety)
- Fix unified oneshot boot completion logic in `oled_unified_anim.c`.
- Fix WPM numeric narrowing in `kbdd/anim.c` so values above 255 render correctly.
- Align public contract for `ANIM_LAYER_TRANSITION` immediately:
  - If not implementing now, deprecate/remove public enum/macro/docs references.

Status: **Done**

### Priority 1 (close declared API gaps)
- Implement missing `widget_*` APIs in `oled_declarative.c`:
  - `widget_validate_config`
  - `widget_force_state`
  - `widget_reset`
  - `widget_get_error`
- Add minimal call-site coverage in examples or keymap-internal smoke usage to ensure symbols link and behavior is sane.

Status: **Done**

### Priority 2 (reduce keymap-local hardcoding)
- Introduce explicit layer count constant tied to `enum layers`.
- Replace ad-hoc ordering arrays with a typed layer animation map table as single source of truth.

Status: **Mostly done** (single-source map in place; layer-count source can still be unified further)

### Priority 3 (module-owned transition orchestration)
- Implement reusable layer transition controller in `oled_utils` with Option 1 semantics (latest-target convergence).
- Keep hooks optional (active layer query, transition effect trigger, pre/post render).
- Migrate `kbdd/anim.c` to this controller while preserving existing entrypoints and render order.

Status: **Done**

### Priority 4 (docs and parity cleanup)
- Update `README.md`, `MIGRATION_GUIDE.md`, and `EXAMPLES.md` to reflect shipped behavior.
- Remove or clearly mark examples that depend on non-implemented features.

Status: **Done**

### Exit criteria (definition of done)
- `qmk compile -kb boardsource/lulu/avr -km kbdd` passes.
- Rapid layer switching converges to latest target without intermediate lock-in.
- Boot animation completion state is correct and reliable.
- WPM values render correctly through expected high ranges.
- Public headers, implementation, and docs are mutually consistent.

## Validation Strategy
- Build target:
  - `qmk compile -kb boardsource/lulu/avr -km kbdd`
- Runtime checks:
  - rapid layer switching (skip intermediate, converge latest)
  - boot animation completion and post-boot triggers
  - modifier toggles under rapid mod changes
  - WPM display correctness up to expected ranges

### Validation Status
- Build and runtime validation listed above still apply.
- Additional required check: verify docs/API parity (`README.md`, migration docs, examples) against shipped behavior.

## Out of Scope (for this effort)
- Reworking art assets or frame data.
- Changing visual style/timing unless required for correctness.
- Switching to strict queued transitions (Option 2).
