#include <cstdint>
#include <array>
#include <iostream>
#include <type_traits> // For std::is_same_v
#include <algorithm>    // For std::copy
#include <iterator>     // For std::begin, std::end

struct myPkt1 {
    uint8_t start = 0x7e;
    uint32_t pktId = 0xbeef1010;
    uint16_t fcs =0;
    uint8_t end = 0x7e;
};

struct myPkt2 {
    float temp = 3.14;
    double pressure = 100.0;
};

template<typename TData>
class Event {
   public:
    constexpr Event(uint16_t id, const TData& data) : eventId(id), eventData(data) {
    }
    uint16_t eventId = 0;
    TData eventData;
};

template<typename TData>
class Error : Event<TData> {
   public:
    Error(uint16_t id, uint16_t error, TData& data) : Event<TData>(id, data), errorCode(error) {
    }
    uint16_t errorCode = 0;
};

using MyEvent1 = Event<myPkt1>;
using MyEvent2 = Event<myPkt2>;
constexpr MyEvent1 MY_EVENT1 = MyEvent1(123, {});
constexpr MyEvent2 MY_EVENT2 = {124, {2.18, 200.0}};
