// g++ -std=c++20 main.cpp -o showPktOrder
#include "hdlcPktDataSwap.h"
int main() {
    uint16_t testData[3] = {0x1234, 0x5678, 0x9abc};

    cout << "Before swapping to network:\n";
    dumpHexArr(testData);

    toNetworkOrder(testData);

    cout << "After swapping to network:\n";
    dumpHexArr(testData);

    FeiLaunchPacket myLaunch = {0xABCD, 0xbeef};
    FeiLaunchPacketHdlc launch = {0x7e00, 0x33, 0x00, {0x9697, 0xcafe}, 0x1234, 0x7e7e};
    FeiLimitPacketHdlc limit = {0x7e00, 0x33, 0x00, {0x4455, {0xcafe, 0xf00d, 0xdead, 0xbeef, 0x55aa}}, 0x1234, 0x7e7e};

    cout << "before lauch to network:\n";
    launch.dumpPacket();
    launch.toNetworkOrder();
    cout << "after lauch to network:\n";
    launch.dumpPacket();
    cout << "before limit to network:\n";
    limit.dumpPacket();
    limit.toNetworkOrder();
    cout << "after limit to network:\n";
    limit.dumpPacket();
    return 0;
}
