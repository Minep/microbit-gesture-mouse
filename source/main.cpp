#include <math.h>
#include "MicroBit.h"
#include "gestures.h"
#include "mdl_wire.h"
#include "lib_images.h"

#define UPDATE_PRE_SECONDS 8
#define SCROLL_THERSHOLD 30
#define SCROLL_UPPER_LIM 90
#define SCROLL_RATIO 0.6
#define SCROLL_BASEVAL 20
#define TILTED_THERSHOLD 30
#define TILTED_UPPER_LIM 90
#define MOUSE_THRESHOLD 10



MicroBit mbit;


ControllerGesture* controllerGesture;
MdlWire* mdlWire;
mdl_vector* direction;
bool isTiltedTakePlace = false;
bool isScrollTakePlace = false;

bool signleUseFlag = false;


void OnScrollAction(int dy)
{
    //mbit.serial.printf("Scrolled:%d\r\n",dy);
    if(abs(dy)>=SCROLL_THERSHOLD && abs(dy)<=SCROLL_UPPER_LIM && !isTiltedTakePlace){
        int rest = controllerGesture->map(
            -SCROLL_UPPER_LIM + SCROLL_THERSHOLD,
            SCROLL_UPPER_LIM - SCROLL_THERSHOLD,
            -5,
            5,
            dy - SCROLL_BASEVAL * (dy < 0 ? -1 : 1)
        );

        mdlWire->enqueue(mdlWire->createPacket(MDL_ONPITCH,rest,0));
        isScrollTakePlace = true;
        if(rest<0){
            mbit.display.print(up);
        }
        else{
            mbit.display.print(down);
        }
        signleUseFlag=false;
    }
    else if(isScrollTakePlace)
    {
        isScrollTakePlace =false;
        signleUseFlag=true;
        if(!(mbit.display.image == img))
        {
            mbit.display.print(img);
        }
    }
}

void OnTilted(int dx)
{
    if(abs(dx)>=TILTED_THERSHOLD && abs(dx)<=TILTED_UPPER_LIM && !isScrollTakePlace){
        int rest = controllerGesture->map(
            -TILTED_UPPER_LIM+TILTED_THERSHOLD,
            TILTED_UPPER_LIM-TILTED_THERSHOLD,
            -5,
            5,dx - TILTED_THERSHOLD * (dx < 0 ? -1 : 1));
        mdlWire->enqueue(mdlWire->createPacket(MDL_ONROLL,rest,0));
        isTiltedTakePlace = true;
        if(rest<0){
            mbit.display.print(right);
        }
        else{
            mbit.display.print(left);
        }
        signleUseFlag=false;
    }
    else if(isTiltedTakePlace)
    {
        isTiltedTakePlace=false;
        signleUseFlag=true;
        if(!(mbit.display.image == img))
        {
            mbit.display.print(img);
        }
    }
}

void OnMouseMove(int x, int y)
{
    if(x>=-90 && x<=90 && y>=-90 && y<=90)
    {
        int valx = controllerGesture->map(-90,90,-10,10,x);
        int valy = controllerGesture->map(-90,90,-10,10,y);
        direction->x = valx;
        direction->y = valy;
        mdlWire->enqueue(mdlWire->createPacket(MDL_ONMOUSEMOVE,valx,valy));
        //mbit.serial.printf("x:%d|y:%d\r\n",valx,valy);
    }
}

void onModeChanged(uint8_t modes)
{
    //mbit.serial.printf("Mode set to :%u\r\n",modes);
    mdlWire->enqueue(mdlWire->createPacket(MDL_MODE_CHANGED,modes,0));
    if(modes == MDL_G_MOUSE)
    {
        mbit.display.print("1");
    }
    else
    {
        mbit.display.print("2");
    }
    mbit.sleep(1000);
    mbit.display.print(img);
}

void OnButtonAEvent(BUTTON_STATUS status)
{
    //mbit.serial.printf("Status changed: %u\r\n",status);
    mdlWire->enqueue(mdlWire->createPacket(MDL_WIN_BTN_CHANGED,status,0));
    if(status == PRESSED)
    {
        mbit.display.print(win);
    }
    else
    {
        mbit.display.print(img);
    }
    
}

void loop()
{
    if(controllerGesture->getModes()==MDL_G_MULT_MOUSE)
    {
        if(controllerGesture->getLeftButtonStatus() == PRESSED)
        {
            if(signleUseFlag)
            {
                OnScrollAction(mbit.accelerometer.getPitch());
                OnTilted(mbit.accelerometer.getRoll());
            }
        }
    }
    else if(controllerGesture->getModes() == MDL_G_MOUSE)
    {
        OnMouseMove(mbit.accelerometer.getRoll(),mbit.accelerometer.getPitch());
    }
}

void loop_send_serial()
{
    while(1)
    {
        //mbit.serial.printf("send all\r\n");
        mdlWire->SendAll();
        mbit.sleep(1000/32);
    }
}

int main()
{
    mbit.init();

    
    controllerGesture = new ControllerGesture();
    mdlWire = new MdlWire(&mbit);
    direction = new mdl_vector();
    
    GESTURE_HANDLERS* handlers = (GESTURE_HANDLERS*) malloc(sizeof(GESTURE_HANDLERS));
    handlers->onBtnAEvent = OnButtonAEvent;
    handlers->onModeChanged=onModeChanged;
    handlers->onScroll = OnScrollAction;

    direction->x=0;
    direction->y=0;

    controllerGesture->InitializeGestureHandlers(handlers);
    mbit.display.print(img);

    mbit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_EVT_ANY, [](MicroBitEvent event)->void{
        controllerGesture->OnButtonClicked(event);
    });
    mbit.messageBus.listen(MICROBIT_ID_BUTTON_B,MICROBIT_BUTTON_EVT_CLICK, [](MicroBitEvent event)->void{
        controllerGesture->OnButtonClicked(event);
    });
    mbit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED,[](MicroBitEvent event)->void{
        mbit.serial.printf("connected \r\n");
        mdlWire->setConnected(true);
    });
    mbit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED,[](MicroBitEvent event)->void{
        mbit.serial.printf("disconnected \r\n");
        mdlWire->setConnected(false);
    });

    create_fiber(loop_send_serial);
    
    while(1){
        loop();
        mbit.sleep(1000/UPDATE_PRE_SECONDS);
    }

    mbit.display.scroll("UNLOADED");
    
    //free(CtlGes);
    //free(handlers);
}