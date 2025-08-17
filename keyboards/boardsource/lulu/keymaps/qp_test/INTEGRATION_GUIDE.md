# QP Utils Integration Guide for qp_test Keymap

This guide explains how to integrate and test the QP Utils module with the qp_test keymap.

## Current Setup

The qp_test keymap is now configured to demonstrate QP Utils functionality with:
- **Display**: SH1106 128x64 monochrome OLED (I2C address 0x3C)
- **Demo System**: Multi-mode demonstration with auto-cycling
- **Integration**: Complete QP Utils module integration

## What's Already Configured

### 1. **Files Added/Modified**
- ✅ `keymap.c` - Updated with QP demo integration
- ✅ `qp_demo.h` - Demo system header
- ✅ `qp_demo.c` - Demo system implementation
- ✅ `rules.mk` - Already configured for Quantum Painter
- ✅ `config.h` - Already configured for I2C

### 2. **QP Utils Module**
- ✅ All QP Utils modules are included in the build
- ✅ Core drawing utilities (`qp_utils`)
- ✅ Image system (`qp_image`)
- ✅ Animation engine (`qp_anim`)
- ✅ Animation controllers (`qp_controllers`)
- ✅ Declarative widgets (`qp_declarative`)

## What You Need to Provide

### 1. **Hardware Setup**
Connect your SH1106 OLED display to the Lulu keyboard:
```
SH1106 OLED -> Lulu
VCC        -> 3.3V
GND        -> GND
SCL        -> SCL (I2C clock)
SDA        -> SDA (I2C data)
```

### 2. **Optional: Custom Images (Advanced)**
If you want to test with real QGF images instead of placeholder shapes:

```bash
# Create directories
mkdir keyboards/boardsource/lulu/keymaps/qp_test/images
mkdir keyboards/boardsource/lulu/keymaps/qp_test/generated

# Add your PNG images to the images/ directory
# Then convert them to QGF format:
qmk painter-convert-graphics -f mono2 -i images/icon1.png -o ./generated/
qmk painter-convert-graphics -f mono2 -i images/icon2.png -o ./generated/
qmk painter-convert-graphics -f mono2 -i images/spinner1.png -o ./generated/
qmk painter-convert-graphics -f mono2 -i images/spinner2.png -o ./generated/

# Include the generated headers in qp_demo.c:
# #include "generated/icon1.qgf.h"
# #include "generated/icon2.qgf.h"
# etc.
```

### 3. **Build and Flash**
```bash
# Build the keymap
qmk compile -kb boardsource/lulu -km qp_test

# Flash to your keyboard
qmk flash -kb boardsource/lulu -km qp_test
```

## Demo Modes

The demo automatically cycles through 4 modes every 10 seconds:

### 1. **Basic Mode**
- Layer indicators (4 boxes, filled = active layer)
- Modifier status (Shift, Ctrl, Alt, GUI)
- WPM bar graph (if WPM_ENABLE is set)
- Caps Lock indicator

### 2. **Animation Mode**
- Simple rotating line animation
- Demonstrates basic frame-based animation
- Shows timing and frame management

### 3. **Widgets Mode**
- Placeholder for declarative widget demonstration
- Shows widget positioning and layout

### 4. **Stress Test Mode**
- Performance test with many small rectangles
- Tests drawing performance and refresh rates

## Manual Controls

- **F13 Key**: Manually cycle through demo modes
- **Any Layer Key**: See layer indicator change in Basic mode
- **Modifiers**: See modifier status in Basic mode
- **Caps Lock**: See caps lock indicator in Basic mode

## Testing Checklist

### ✅ **Basic Functionality**
- [ ] Display initializes and shows content
- [ ] Header bar appears at top
- [ ] Status bar appears at bottom with mode indicators
- [ ] Main content area shows demo content

### ✅ **Demo Modes**
- [ ] Basic mode shows layer and modifier indicators
- [ ] Animation mode shows rotating line
- [ ] Widgets mode shows placeholder widgets
- [ ] Stress test mode shows many rectangles
- [ ] Modes auto-cycle every 10 seconds

### ✅ **Interactive Features**
- [ ] Layer changes update layer indicator
- [ ] Modifier keys update modifier status
- [ ] Caps Lock toggles caps indicator
- [ ] F13 key manually cycles modes

### ✅ **Performance**
- [ ] Display updates smoothly (50ms intervals)
- [ ] No visible flickering or tearing
- [ ] Responsive to keyboard input
- [ ] Stable operation over time

## Troubleshooting

### Display Not Working
1. **Check Hardware Connections**
   - Verify I2C wiring (SDA, SCL, VCC, GND)
   - Ensure display is powered (3.3V)
   - Check I2C address (default 0x3C)

2. **Check Configuration**
   - Verify `QUANTUM_PAINTER_ENABLE = yes` in rules.mk
   - Verify `QUANTUM_PAINTER_DRIVERS += sh1106_i2c` in rules.mk
   - Check I2C pins in config.h

3. **Debug Steps**
   ```c
   // Add to keyboard_post_init_user() for debugging
   if (!qp_demo_init()) {
       // Display initialization failed
       // Check hardware and configuration
   }
   ```

### Performance Issues
1. **Reduce Update Frequency**
   - Increase update interval in qp_demo_update()
   - Reduce complexity in stress test mode

2. **Check I2C Speed**
   - Verify I2C clock speed in config.h
   - Try reducing speed if experiencing issues

### Build Errors
1. **Missing QP Utils Module**
   - Ensure qp_utils module is in modules/dmyoung9/qp_utils/
   - Check that all .h and .c files are present

2. **Include Path Issues**
   - Verify SRC paths in rules.mk
   - Check include statements in source files

## Next Steps

### 1. **Add Real Images**
- Create PNG images for icons and animations
- Convert to QGF format using QMK CLI
- Update demo code to use real images

### 2. **Implement Advanced Features**
- Add declarative widget examples
- Implement animation controllers
- Add more complex animations

### 3. **Customize for Your Use Case**
- Modify demo modes for your specific needs
- Add custom query functions
- Integrate with your keymap features

### 4. **Performance Optimization**
- Profile drawing operations
- Optimize update frequencies
- Implement selective redrawing

## Example Customization

```c
// Add to qp_demo.c for custom functionality
void qp_demo_draw_custom_widget(void) {
    // Your custom drawing code here
    uint16_t x = 10;
    uint16_t y = DEMO_MAIN_Y + 30;
    
    // Example: Draw typing speed indicator
    uint8_t wpm = get_current_wpm();
    uint16_t bar_width = (wpm * 50) / 100; // Scale to 50px max
    
    qp_draw_rect(display, x, y, 52, 8, QP_COLOR_WHITE);
    if (bar_width > 0) {
        qp_fill_rect(display, x + 1, y + 1, bar_width, 6, QP_COLOR_WHITE);
    }
}
```

## Support

If you encounter issues:
1. Check the QP Utils documentation in `modules/dmyoung9/qp_utils/README.md`
2. Review examples in `modules/dmyoung9/qp_utils/EXAMPLES.md`
3. Check performance guidelines in `modules/dmyoung9/qp_utils/PERFORMANCE.md`
4. Verify hardware connections and configuration
