#include <stdint.h>

#define MDL_G_MOUSE 0x00
#define MDL_G_MULT_MOUSE 0xff

typedef enum BUTTON_STATUS
{
    PRESSED = 0x00,
    RELEASED = 0x01,
}BUTTON_STATUS;

typedef struct GESTURE_HANDLERS{
    void (*onScroll)(int);
    void (*onModeChanged)(uint8_t);
    // invoked when mouse moved.
    // arg 1: dx
    // arg 2: dy
    void (*onMouseMove)(double,double);
    void (*onBtnAEvent)(BUTTON_STATUS);
    void (*onBtnABEvent)(BUTTON_STATUS);
}GESTURE_HANDLERS;