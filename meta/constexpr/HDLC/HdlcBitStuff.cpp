#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstdint>


// Helper to extract a single bit from a byte buffer at a given bit index
bool get_bit(const uint8_t* buffer, size_t bit_index) {
    size_t byte_index = bit_index / 8;
    int bit_offset = bit_index % 8; // LSB first due to UART TX order
    return (buffer[byte_index] >> bit_offset) & 0x01;
}

// Helper to append a single bit to the output buffer
void put_bit(uint8_t* buffer, size_t bit_index, bool bit) {
    size_t byte_index = bit_index / 8;
    int bit_offset = bit_index % 8;
    if (bit) {
        buffer[byte_index] |= (0x01 << bit_offset);
    } else {
        buffer[byte_index] &= ~(0x01 << bit_offset);
    }
}

// Stuffs data from input_buffer to output_buffer.
// Returns the total number of stuffed bits.
size_t hdlc_bit_stuff(const uint8_t* input_buffer, size_t input_bit_length, uint8_t* output_buffer) {
    size_t out_bit_idx = 0;
    int consecutive_ones = 0;

    for (size_t i = 0; i < input_bit_length; ++i) {
        bool bit = get_bit(input_buffer, i);

        if (bit) {
            consecutive_ones++;
        } else {
            consecutive_ones = 0;
        }

        // Put the current bit
        put_bit(output_buffer, out_bit_idx++, bit);

        // If we have 5 consecutive ones, stuff a 0 bit immediately
        if (consecutive_ones == 5) {
            put_bit(output_buffer, out_bit_idx++, false);
            consecutive_ones = 0; // Reset counter after stuffing
        }
    }
    return out_bit_idx; // Total bits in the stuffed buffer
}

// Destuffs data from stuffed_buffer to output_buffer.
// Returns the total number of destuffed bits.
size_t hdlc_bit_destuff(const uint8_t* stuffed_buffer, size_t stuffed_bit_length, uint8_t* output_buffer) {
    size_t out_bit_idx = 0;
    int consecutive_ones = 0;

    for (size_t i = 0; i < stuffed_bit_length; ++i) {
        bool bit = get_bit(stuffed_buffer, i);

        if (bit) {
            consecutive_ones++;
        } else {
            consecutive_ones = 0;
        }

        // Put the current bit into destination
        put_bit(output_buffer, out_bit_idx++, bit);

        // If we found 5 ones, the next bit MUST be a stuffed 0
        if (consecutive_ones == 5) {
            if (i + 1 < stuffed_bit_length) {
                // Skip the stuffed zero bit
                i++; 
            }
            consecutive_ones = 0; // Reset
        }
    }
    return out_bit_idx;
}

std::string hexDataStr(const void *buffer, size_t N) {
    const uint8_t* ptr = static_cast<const uint8_t*>(buffer);
    std::stringstream ss("");
    bool first = true;
    for (size_t i=0; i<N; ++i) {
        if (!first) {
            if (i%16 == 0) {
                ss << '\n';
            } else if (i%8 == 0) {
                ss << "  ";
            }
        }
        ss << std::setfill('0') << std::setw(2) << std::right << std::hex;
        ss << uint16_t(ptr[i]) << ' ';
        first = false;
    }
    return ss.str();
}

size_t bufferSize(size_t bitSize) {
    size_t retVal = bitSize / 8;
    return retVal + (bitSize % 8);
}

constexpr size_t maxStuffBuffer(size_t bitSize) {
    return bitSize + bitSize / 5;
}

int main() {
    // Raw binary data (contains five consecutive ones: 0x7C is 01111100)
    uint8_t input_data[] = {0x7C, 0xff}; 
    size_t input_bits = 16; // Let's process the first 12 bits: 011111000000

    // Stuffed array can be slightly larger, allocate safely
    uint8_t stuffed_data[maxStuffBuffer(16)] = {0}; 
    size_t stuffed_len = hdlc_bit_stuff(input_data, input_bits, stuffed_data);

    // Destuff back to another array
    uint8_t destuffed_data[3] = {0};
    size_t destuffed_len = hdlc_bit_destuff(stuffed_data, stuffed_len, destuffed_data);

    std::cout << "Original bits:" << hexDataStr(input_data, 2) << "\n";
    std::cout << "Total Stuffed Bits: " << stuffed_len << ":" << hexDataStr(stuffed_data, bufferSize(stuffed_len)) << "\n";
    std::cout << "Total Destuffed Bits: " << destuffed_len << ":" << hexDataStr(destuffed_data, bufferSize(destuffed_len)) << "\n";

    return 0;
}

