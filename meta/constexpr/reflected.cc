#include <iostream>
#include <vector>
#include <stdint.h>
#include <iomanip>

class CRC16HDLC {
private:
    uint16_t table[256];
    const uint16_t POLYNOMIAL = 0x8408; // Reversed 0x1021

public:
    CRC16HDLC() {
        // Generate the table
        for (uint16_t i = 0; i < 256; i++) {
            uint16_t crc = i;
            for (uint8_t j = 0; j < 8; j++) {
                if (crc & 0x0001)
                    crc = (crc >> 1) ^ POLYNOMIAL;
                else
                    crc = (crc >> 1);
            }
            table[i] = crc;
            printf("0x%04x%s", table[i], (i%8==7) ? ",\n" : ", ");
        }
    }

    uint16_t calculate(const std::vector<uint8_t>& data, uint16_t initial_crc = 0x0000) {
        uint16_t crc = initial_crc;
        for (uint8_t byte : data) {
            // Reflected input calculation
            crc = (crc >> 8) ^ table[(crc ^ byte) & 0xFF];
        }
        return crc;
    }
};

int main() {
    // Example Usage
    CRC16HDLC crc16;
    // Data: "123456789"
    std::vector<uint8_t> data = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
    
    // Initial Seed: 0x0000 per request
    uint16_t result = crc16.calculate(data, 0x0000);
    
    std::cout << "CRC-16 (0x1189) result: 0x" 
              << std::hex << std::setw(4) << std::setfill('0') 
              << result << std::endl;
              
    // Note: If you need it for standard HDLC/SDLC, 
    // the initial value is usually 0xFFFF.

    return 0;
}

