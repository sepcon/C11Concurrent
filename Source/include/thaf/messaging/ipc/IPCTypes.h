#pragma once

namespace thaf {
namespace messaging {
namespace ipc {

enum class IPCType : unsigned char
{
    Local,
    Domain,
    //Somethings else will be defined here
    Invalid
};


} // ipc
} // messaging
} // thaf
