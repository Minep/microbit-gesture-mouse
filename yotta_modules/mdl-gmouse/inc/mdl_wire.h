#include <stdint.h>
#include <queue>

#include "MicroBit.h"

#define MDL_WIN_BTN_CHANGED 0x01
#define MDL_MODE_CHANGED 0X02
#define MDL_ONROLL 0x03
#define MDL_ONPITCH 0x04
#define MDL_ONMOUSEMOVE 0x05

#pragma pack(push,1)

typedef struct MDL_MOUSE_PACKET
{
    uint8_t command;  
    int8_t signed value1;
    int8_t signed value2;
} mdl_mouse_packet;
typedef struct MDL_VECTOR
{
    int8_t signed x;
    int8_t signed y;
} mdl_vector;

#pragma pack(pop)

#define MDL_PACKET_LEN sizeof(mdl_mouse_packet)

// 5004638978

class MdlWire{
    public:
        MdlWire(MicroBit* mbit);
        void enqueue(mdl_mouse_packet* packet);
        void SendAll();
        void Send(uint8_t* packet);
        void setConnected(bool connect);
        bool getConnected();
        mdl_mouse_packet* createPacket(uint8_t cmd,int val1,int val2);
    private:
        std::queue<uint8_t*> queue;
        MicroBit* bit;
        bool isConnected;

};