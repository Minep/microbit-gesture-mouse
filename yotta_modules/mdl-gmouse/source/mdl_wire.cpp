#include "mdl_wire.h"
#include <stdlib.h>
#include <string.h>


MdlWire::MdlWire(MicroBit* mbit)
{
    bit = mbit;
}

void MdlWire::enqueue(mdl_mouse_packet* packet)
{
    uint8_t* bpacket = new uint8_t[MDL_PACKET_LEN];
    memcpy(bpacket,packet,MDL_PACKET_LEN);
    queue.push(bpacket);
    free(packet);
}

void MdlWire::Send(uint8_t* packet)
{
    if(!queue.empty()){
        bit->serial.send(packet,MDL_PACKET_LEN,SYNC_SLEEP);
    }
}

void MdlWire::SendAll()
{
    if(!queue.empty()){
        for(int i=0;i<queue.size();i++){
            uint8_t* data = queue.front();
            Send(data);
            queue.pop();
            delete data;
        }
    }
}

mdl_mouse_packet* MdlWire::createPacket(uint8_t cmd,int val1,int val2)
{
    mdl_mouse_packet* packet = (mdl_mouse_packet*)calloc(1,MDL_PACKET_LEN);
    packet->command = cmd;
    packet->value1 = val1;
    packet->value2 = val2;
    return packet;
}

void MdlWire::setConnected(bool conn)
{
    isConnected = conn;
}

bool MdlWire::getConnected()
{
    return isConnected;
}