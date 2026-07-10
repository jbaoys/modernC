#ifndef _HDLCPKTDATASWAP_H
#define _HDLCPKTDATASWAP_H
#include <iostream>
#include <iomanip>
#include <cstdint>
#include <arpa/inet.h>
#include <type_traits>
#include <bit>

#define PACKED __attribute__((packed))

using namespace std;

template<typename TData>
struct HexStrF {
    const TData &dtRef;

    HexStrF(TData dt):dtRef(dt){
    }

    friend ostream& operator<<(ostream& os, const HexStrF& hStr) {
        os << "\"0x" << hex << uppercase << setfill('0') << setw(sizeof(TData)) << static_cast<uint64_t>(hStr.dtRef) << "\"" << dec;
        return os;
    }
};

template<typename TData>
struct HexStr {
    const TData value;

    HexStr(TData dt):value(dt){
    }

    ostream& output(ostream& os) const {
        os << "\"0x" << hex << uppercase << setw(sizeof(TData)*2) << setfill('0') << static_cast<uint64_t>(value) << "\"" << dec;
        return os;
    }
};

template<typename TData>
ostream& operator<<(ostream& os, const HexStr<TData>& td) {
    return td.output(os);
}


// Primary template
template<typename T>
T toNetworkOrder(T value) = delete; // Disable non-supported types

// Specializeation for 16-bit
template<>
inline uint16_t toNetworkOrder(uint16_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return __builtin_bswap16(value); // or htons(value);
    }
    return value;
}


// Specializeation for 32-bit
template<>
inline uint32_t toNetworkOrder(uint32_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return __builtin_bswap32(value); // or htonl(value);
    }
    return value;
}


// Specializeation for 64-bit
template<>
inline uint64_t toNetworkOrder(uint64_t value) {
    if constexpr (std::endian::native == std::endian::little) {
        return __builtin_bswap64(value);
    }
    return value;
}


template<typename T, size_t N>
void toNetworkOrder(T (&data)[N]) {
    if constexpr (std::endian::native == std::endian::little) {
        for (int i=0; i<N; ++i) {
            data[i] = toNetworkOrder(data[i]);
        }
    }
}

template<typename T, size_t N> 
void dumpHexArr(const T (&data)[N]) {
    for (int i = 0; i<N; ++i) {
        cout << " .[" << i << "] = " << HexStr(data[i]) << '\n';
    }
}


struct FeiCmdPacket {
    uint16_t cmdId;
} PACKED;

struct FeiNopPacket : FeiCmdPacket {
} PACKED;

struct FeiLaunchPacket : FeiCmdPacket {
    uint16_t launch;
} PACKED;

struct limits {
    uint16_t a;
    uint16_t b;
    uint16_t c;
    uint16_t d;
    uint16_t e;
};

struct FeiLimitPacket : FeiCmdPacket {
    limits lmt;
} PACKED;

template<typename T>
struct FeiCmdPacketWrapper : T {
    void convertToNetworkOrder() {
        constexpr size_t N = sizeof(T) / sizeof(uint16_t);
        uint16_t (&arr)[N] = reinterpret_cast<uint16_t(&)[N]>(T::cmdId);
        ::toNetworkOrder(arr);
    }
} PACKED;

template <typename TPkt, typename TFlag = uint16_t>
struct FeiHdlcPacket {
    // 1. Asserts no custom/user-provided constructors exist
    static_assert(std::is_trivially_default_constructible_v<TPkt>, 
                  "Error: TPkt must not contain custom constructors!");

    // 2. Asserts no custom/user-provided destructor exists
    static_assert(std::is_trivially_destructible_v<TPkt>, 
                  "Error: TPkt must not contain a custom destructor!");
    TFlag start;
    uint8_t address;
    uint8_t control;
    TPkt info;
    uint16_t fcs;
    TFlag end;

    void toNetworkOrder() {
        info.convertToNetworkOrder();
    }

    void dumpPacket() {
        cout << "start: " << HexStr(start) << "\n";
        cout << "address: " << HexStr(address) << "\n";
        cout << "control: " << HexStr(control) << "\n";
        cout << "info:\n";
        constexpr size_t N = sizeof(info) / sizeof(uint16_t);
        uint16_t (&arr)[N] = reinterpret_cast<uint16_t(&)[N]>(info.cmdId);
        dumpHexArr(arr);
        cout << "fcs: " << HexStr(fcs) << "\n";
        cout << "stop: " << HexStr(end) << "\n";
    }

} PACKED;

using FeiLaunchInfoPkt = FeiCmdPacketWrapper<FeiLaunchPacket>;
using FeiLaunchPacketHdlc = FeiHdlcPacket<FeiLaunchInfoPkt>;
    // 1. Asserts no custom/user-provided constructors exist
    static_assert(std::is_trivially_default_constructible_v<FeiLaunchPacketHdlc>, 
                  "Error: Derived class must not contain custom constructors!");

    // 2. Asserts no custom/user-provided destructor exists
    static_assert(std::is_trivially_destructible_v<FeiLaunchPacketHdlc>, 
                  "Error: Derived class must not contain a custom destructor!");
using FeiLimitPacketHdlc = FeiHdlcPacket<FeiCmdPacketWrapper<FeiLimitPacket>, uint16_t>;

#endif //_HDLCPKTDATASWAP_H
