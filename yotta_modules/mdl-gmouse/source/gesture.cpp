#include "gestures.h"

ControllerGesture::ControllerGesture(void)
{
    
}

void ControllerGesture::OnButtonClicked(MicroBitEvent bitEvent)
{
    uint16_t buttonID = bitEvent.source;
    switch (bitEvent.value)
    {
        case MICROBIT_BUTTON_EVT_DOWN:
            if(buttonID == MICROBIT_ID_BUTTON_A){
                mouse_L_btn = PRESSED;
                handlers->onBtnAEvent(mouse_L_btn);
            }
            else if(buttonID == MICROBIT_ID_BUTTON_AB){
                lrbtn = PRESSED;
            }
            break;
        case MICROBIT_BUTTON_EVT_UP:
            if(buttonID == MICROBIT_ID_BUTTON_A){
                mouse_L_btn = RELEASED;
                handlers->onBtnAEvent(mouse_L_btn);
            }
            else if(buttonID == MICROBIT_ID_BUTTON_AB){
                lrbtn = RELEASED;
            }
            break;
        case MICROBIT_BUTTON_EVT_CLICK:
            if(buttonID == MICROBIT_ID_BUTTON_B){
                modes = ~modes;
                handlers->onModeChanged(modes);
            }
            else if(buttonID == MICROBIT_ID_BUTTON_AB){
                lrbtn = PRESSED;
            }
            break;
        default:
            break;
    }
    // if(buttonID==MICROBIT_ID_BUTTON_A){
    //     handlers->onBtnAEvent(win_btn);    
    // }
    // else if(buttonID==MICROBIT_ID_BUTTON_AB){
    //     handlers->onBtnABEvent(lrbtn);
    // }
}

void ControllerGesture::OnAccmeterUpdate(MicroBitEvent bitevent)
{
    if(bitevent.source == MICROBIT_ACCELEROMETER_EVT_DATA_UPDATE){
        //double dy = map(-90,90,0,240,accr);
    }
}

BUTTON_STATUS ControllerGesture::getLeftButtonStatus()
{
    return mouse_L_btn;
}

uint8_t ControllerGesture::getModes()
{
    return modes;
}

void ControllerGesture::InitializeGestureHandlers(GESTURE_HANDLERS* handle)
{
    handlers = handle;
}

int ControllerGesture::map(int fromLow, int fromHigh, int toLow, int toHigh, int value)
{
    return (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow) + toLow;
}

