/**
 * This is function used to parse a message from a server response.
 * Its task is to extract all data fields from the message and store them into a
 * structure. There are multiple types of messages that can be parsed. The
 * parsing results should be automatically saved to the predefined data
 * structures.
 *
 */
#include <stdio.h>
#include <string.h>
// include header for uint8_t
#include <stdint.h>

#include "ParsingMessage.h"

struct commonHeader {
    uint8_t type;
    uint8_t subType;
    uint8_t infoSize;
    uint8_t info[255];
};

struct DataA {
    uint8_t flag;
    uint16_t sendor1;
    uint16_t sendor2;
    uint32_t fpgaVersion;
};

struct DataB {
    uint32_t monitoringTime;
    uint16_t adc1;
    uint8_t powerOnMask;
};

enum DataTypes : uint8_t{
    DATA_A = 0x01,
    DATA_B = 0x02
};

// Global variables
DataA dataA;
DataB dataB;


void ParsingMessage::parseMessage(uint8_t *message) {
    // The passed in message contiains header and payload
    // First, parse the header and find out the DataTypes
    // Then, parse the payload according to the DataTypes
    commonHeader* header = reinterpret_cast<commonHeader*>(message);

    // Parse the header
    switch (header->type) {

        case DATA_A: {
            // Parse DataA
            // Parse the payload
            dataA = *reinterpret_cast<DataA*>(message);
            break;
        }

        case DATA_B: {
            // Parse DataB
            // Parse the payload
            dataB = *reinterpret_cast<DataB*>(message);
            break;
        }

        default:
            // Unknown message
            // Log error message to stderr
            fprintf(stderr, "Unknown message type: %d\n", header->type);
            break;
    }
}

int main(int argc, char *argv[]) {
    // prepare test messages
    //
    uint8_t message1[] = {DATA_A, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};
    uint8_t message2[] = {DATA_B, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c};

    // parse the messages
    ParsingMessage::parseMessage(message1);
    ParsingMessage::parseMessage(message2);

    // output the results
    printf("DataA: flag = %d, sendor1 = %d, sendor2 = %d, fpgaVersion = %d\n",
           dataA.flag, dataA.sendor1, dataA.sendor2, dataA.fpgaVersion);
    printf("DataB: monitoringTime = %d, adc1 = %d, powerOnMask = %d\n",
           dataB.monitoringTime, dataB.adc1, dataB.powerOnMask);

    return 0;
}

