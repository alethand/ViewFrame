#include "basetransmit.h"

namespace Network{

BaseTransmit::BaseTransmit():netConnected(false)
{

}

BaseTransmit::~BaseTransmit()
{

}

bool BaseTransmit::connected()
{
    return netConnected;
}

}
