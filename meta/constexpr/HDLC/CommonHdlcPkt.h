#ifndef COMMONHDLCPKT_H
#define COMMONHDLCPKT_H
#include <array>
#include <cstdint>
/**
 * note: CRC-16-CCITT reflected polynominal value of 0x8408 (non-relected 0x1021)
 */
template<typename TPayload, typename TFrameFlag, uint16_t fcsSeed = 0xFFFF, uint16_t polynominal = 0x8408/*0x1021*/>
struct __attribute__((packed)) commonHdlcPkt {
    static_assert(std::is_trivially_copyable<TPayload>::value, "TPayload must be trivially copyable");
    TFrameFlag startFlag = 0;
    uint8_t address = 0;
    uint8_t control = 0;
    TPayload payload = {};
    uint16_t fcs = 0;
    TFrameFlag endFlag = 0;

    constexpr commonHdlcPkt(TFrameFlag start, uint8_t addr, uint8_t ctrl, TPayload pay, TFrameFlag end)
    : startFlag(start),
      address(addr),
      control(ctrl),
      payload(pay),
      endFlag(end) {
    }

    constexpr static auto crc16_table = []() constexpr {
        std::array<uint16_t, 256> table{};
        // Generate the table
        for (uint16_t i = 0; i < 256; i++) {
            uint16_t crc = i;
            for (uint8_t j = 0; j < 8; j++) {
                if (crc & 0x0001)
                    crc = static_cast<uint16_t>((crc >> 1) ^ polynominal);
                else
                    crc = static_cast<uint16_t>(crc >> 1);
            }
            table[i] = crc;
        }
        return table;
    }();

    void showTable() const {
        for (int i = 0; i < 256; i++) {
            printf("0x%04x%s", crc16_table[i], (i%8==7) ? ",\n" : ", ");
        }
    }

    uint16_t getFcs() const {
        /**
         * HDLC/PPP 16-bit CRC table (polynomial 0x1021)
            static const uint16_t crc16_table[256] = {
                0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
                0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
                0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
                0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
                // ... (table omitted for brevity, full table needed in practice)
                // You can generate this table using a simple algorithm or find it online
            };
         */
        uint16_t fcsVal = fcsSeed;
        const uint8_t* data = reinterpret_cast<const uint8_t*>(&payload);

        for (int i = 0; i < sizeof(TPayload); ++i) {
            // Reflected input calculation
            fcsVal = static_cast<uint16_t>(fcsVal >> 8) ^ crc16_table[(fcsVal ^ data[i]) & 0xFF];
        }

        return fcsVal;
    }

    void updateFcs() {
        fcs = getFcs();
    }
};


template<typename TPayload>
struct __attribute__((packed)) FeiHdlcPkt : commonHdlcPkt<TPayload, uint16_t> {
    constexpr FeiHdlcPkt(TPayload pay)
    : commonHdlcPkt<TPayload, uint16_t>(0x7E00, 0x33, 0x00, pay, 0x7E7E) {
    }

    constexpr FeiHdlcPkt()
    : commonHdlcPkt<TPayload, uint16_t>(0x7E00, 0x33, 0x00, {}, 0x7E7E) {
    }
};

#endif //COMMONHDLCPKT_H
