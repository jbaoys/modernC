#include <iostream>
#include <iomanip>
#include <cstdint>
using namespace std;

template<typename TData>
struct HexStrF {
    const TData& dtRef;

    HexStrF(TData& dt):dtRef(dt){
    }

    friend ostream& operator<<(ostream& os, const HexStrF& hStr) {
        os << "\"0x" << hex << uppercase << setfill('0') << setw(sizeof(TData)) << static_cast<uint64_t>(hStr.dtRef) << "\"" << dec;
        return os;
    }
};

template<typename TData>
struct HexStr {
    const TData& dtRef;

    HexStr(TData& dt):dtRef(dt){
    }

    ostream& output(ostream& os) const {
        os << "\"0x" << hex << uppercase << setw(sizeof(TData)*2) << setfill('0') << static_cast<uint64_t>(dtRef) << "\"" << dec;
        return os;
    }
};

template<typename TData>
ostream& operator<<(ostream& os, const HexStr<TData>& td) {
    return td.output(os);
}

int main() {
    uint8_t a = 10;
    uint32_t b = 0x12345678;
    uint64_t c = 0xcafef00beefu;
    cout << "a_hex:" << HexStr(a) << ", a:" << static_cast<int>(a) << '\n';
    cout << "b_hex:" << HexStr(b) << ", b:" << b << '\n';
    cout << "c_hex:" << HexStr(c) << ", c:" << c << '\n';
}
