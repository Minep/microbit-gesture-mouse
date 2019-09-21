#include "handlers.h"
#include "MicroBit.h"

class ControllerGesture{
    public:
        ControllerGesture();
        void InitializeGestureHandlers(GESTURE_HANDLERS* handle);
        void OnAccmeterUpdate(MicroBitEvent bitevent);
        void OnButtonClicked(MicroBitEvent bitevent);
        int map(int fromLow, int fromHigh, int toLow, int toHigh, int value);
        uint8_t getModes();
        BUTTON_STATUS getWinButtonStatus();
        BUTTON_STATUS getModeButtonStatus();

        // Mouse mode only

        BUTTON_STATUS getLeftButtonStatus();
        BUTTON_STATUS getRightButtonStatus();
    private:
        BUTTON_STATUS win_btn;
        BUTTON_STATUS lrbtn;
        GESTURE_HANDLERS* handlers;
        uint8_t modes = MDL_G_MULT_MOUSE;
};