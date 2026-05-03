#include <cmath>
#include <iostream>
#include <string>

#include "ConstexprConfig.h"
#include "CommonHdlcPkt.h"

struct __attribute__((packed)) NopPkt {
    uint16_t cmdId = 0;
    uint32_t param = 0;
};

using FeiPkt = FeiHdlcPkt<NopPkt>;
FeiPkt myFeiPkt = {{0x3333, 0x1234}};
FeiPkt myFeiPkt2 = {};
constexpr FeiPkt myFeiPkt3 = {{0xCAFE, 0xdeadbeef}};
uint16_t pkt3_fcs = myFeiPkt3.getFcs(); //runtime
FeiPkt myFeiPkt4 = myFeiPkt3;

constexpr double const_sqrt(double x) {
    return sqrt(x);
}
/*
 * Now you can use factorial( 2 ) and when the compiler sees it, it can optimize away the call and make the calculation entirely at compile time. In this way, by allowing more sophisticated calculations, constexpr behaves differently than a mere inline function. You can't inline a recursive function! In fact, any time the function argument is itself a constexpr, it can be computed at compile time.
 */
constexpr int factorial(int n) {
    return n > 0 ? n * factorial( n - 1 ) : 1;
}

constexpr int myConstInt = 123;

void showFeiPkt(const FeiPkt &pkt) {
    printf("pkt START = 0x%04X, ADDR = 0x%02X, CTRL = 0x%02X, ID = 0x%04X, PARAM = 0x%08X, FCS = 0x%04X, END = 0x%04X\n",
            pkt.startFlag, pkt.address, pkt.control, pkt.payload.cmdId, pkt.payload.param, pkt.fcs, pkt.endFlag);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << argv[0]
            << "Version " << Constexpr_VERSION_MAJOR << "."
            << Constexpr_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }
    const double inputValue = std::stod(argv[1]);
    constexpr double inputValue2 = 29424091;
    //This will cause error:
    // error: the value of ‘inputValue’ is not usable in a constant expression
    //constexpr double outputValue = const_sqrt(inputValue);
    constexpr double outputValue = const_sqrt(inputValue2);
    std::cout << "The root of " << inputValue2 << " is " << outputValue << '\n';
    std::cout << "The root of " << argv[1] << " is " << sqrt(inputValue) << '\n';

    constexpr int v = 7;
    constexpr int f = factorial(v);
    std::cout << v << "! = " << f << '\n';

    std::cout << "myConstInt = " << myConstInt << "\n";

    //myConstInt = 321; //error: assignment of read-only variable 

    std::cout << "myConstInt = " << myConstInt << "\n";

    std::cout << "myFeiPkt crc16_table:\n";
    myFeiPkt.showTable();


    printf("myFeiPkt3 FCS = 0x%04X\n", pkt3_fcs);
    myFeiPkt.updateFcs();
    myFeiPkt2.updateFcs();
    myFeiPkt4.updateFcs();
    showFeiPkt(myFeiPkt);
    showFeiPkt(myFeiPkt2);
    showFeiPkt(myFeiPkt3);
    showFeiPkt(myFeiPkt4);
    return 0;
}
