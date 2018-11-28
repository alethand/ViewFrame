#include "udp_bytedatapacketrule.h"

namespace Network{

UDP_ByteDataPacketRule::UDP_ByteDataPacketRule()
{

}

UDP_ByteDataPacketRule::~UDP_ByteDataPacketRule()
{

}

bool UDP_ByteDataPacketRule::wrap(const SendUnit &sunit, UDPByteSender sendDataFunc)
{
    return true;
}

bool UDP_ByteDataPacketRule::unwrap(const char *data, const int length, DataHandler handler)
{
    return true;
}

void UDP_ByteDataPacketRule::registDataHandler(Handler *dataHandler)
{

}

} //namespace Network
