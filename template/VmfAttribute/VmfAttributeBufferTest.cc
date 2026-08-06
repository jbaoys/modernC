#include "VmfAttributeBuffer.h"
#include <gmock/gmock.h>  // Brings in gMock.
#include <gtest/gtest.h>



TEST(VmfAttributeBufferTest, ENDIAN) {
    if constexpr(isBigEndian) {
        EXPECT_TRUE(isBigEndian);
        VmfDebug("THIS TEST DETECT THE ENDIAN AS BIG ENDIAN!\n");
    } else {
        EXPECT_FALSE(isBigEndian);
        VmfDebug("THIS TEST DETECT THE ENDIAN AS LITTLE ENDIAN!\n");
    }
}

TEST(VmfAttributeBufferTest, Constructor) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 256);
    EXPECT_EQ(256, myBuffer.getSize());
    EXPECT_EQ(0, myBuffer.getContentSize());
}

TEST(VmfAttributeBufferTest, data64bits) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 256);

    VmfAttributeBuffer::data64bits d64(0xa123456789abcdef);
    swap_endian(d64.U64);
    EXPECT_EQ(0xefcdab89674523a1, d64.U64);
    swap_endian(d64.U64);
    EXPECT_EQ(0xa123456789abcdef, d64.U64);
    if constexpr(isBigEndian) {
        EXPECT_EQ(0xa1, d64.U8[u64LowByte]);
    } else {
        VmfDebug("LITTLE ENDIAN data64bits tests:\n");
        EXPECT_EQ(0xa1, d64.U8[u64HighByte]);
        EXPECT_EQ(0xef, d64.U8[u64LowByte]);
        d64.leftShift(4);
        EXPECT_EQ(0x12, d64.U8[u64HighByte]);
        EXPECT_EQ(0xf0, d64.U8[u64LowByte]);
        EXPECT_EQ(0x0a, d64.U8[sizeof(uint64_t)]);
        d64.rightShift(4);
        EXPECT_EQ(0xa1, d64.U8[u64HighByte]);
        EXPECT_EQ(0xef, d64.U8[u64LowByte]);
    }
}

TEST(VmfAttributeBufferTest, getContentSize) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 256);
    EXPECT_EQ(256, myBuffer.getSize());
    EXPECT_EQ(0, myBuffer.getContentSize());
    uint8_t bits[] = { 0, 1, 1, 0, 0, 0,1,0,1};
    for (auto b : bits) {
        EXPECT_TRUE(myBuffer.appendBit(b));
    }
    EXPECT_EQ(2, myBuffer.getContentSize());
    EXPECT_EQ(9, myBuffer.getContentBitSize());
}

TEST(VmfAttributeBufferTest, appendBit) {
    uint8_t buf[256];
    memset(buf, 0, 256);
    VmfAttributeBuffer myBuffer(buf, 2);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x01, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x03, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_EQ(0x03, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_EQ(0x03, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x13, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_EQ(0x13, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_EQ(0x13, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x93, buf[0]);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_EQ(0x93, buf[0]);
    EXPECT_EQ(0x01, buf[1]);
    EXPECT_EQ(2, myBuffer.getContentSize());
    for (int i = 0; i < 7; i++) {
        EXPECT_TRUE(myBuffer.appendBit(1));
    }
    EXPECT_FALSE(myBuffer.appendBit(1));
}

TEST(VmfAttributeBufferTest, appendBits) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer(buf, 2);
    EXPECT_FALSE(myBuffer.appendBits(0x8877665544332211, 17));
    myBuffer.purgeBuffer();
    EXPECT_TRUE(myBuffer.appendBits(0x8877665544332211, 16));
    myBuffer.purgeBuffer();
    EXPECT_TRUE(myBuffer.appendBits(0x8877665544332211, 1));
    EXPECT_TRUE(myBuffer.appendBits(0x8877665544332211, 0));
    VmfAttributeBuffer myBuffer2(buf, 3);
    EXPECT_FALSE(myBuffer2.setContentBitSize(25));
    EXPECT_TRUE(myBuffer2.setContentBitSize(24));
    EXPECT_TRUE(myBuffer2.setContentBitSize(23));
    myBuffer2.resetIterator();
    EXPECT_TRUE(myBuffer2.appendBits(0x8877665544332211, 0));
    myBuffer2.purgeBuffer();
    EXPECT_TRUE(myBuffer2.appendBits(0x8877665544332211, 23));
    myBuffer2.purgeBuffer();
    EXPECT_FALSE(myBuffer2.appendBits(0x8877665544332211, 25));
    myBuffer2.purgeBuffer();
    EXPECT_TRUE(myBuffer2.appendBits(0x8877665544332211, 24));
    EXPECT_EQ(3, myBuffer2.getContentSize());
    EXPECT_EQ(0x11, buf[0]);
    EXPECT_EQ(0x22, buf[1]);
    EXPECT_EQ(0x33, buf[2]);
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer2.getContentSize()));

    VmfAttributeBuffer myBuffer3(buf, 8);
    EXPECT_TRUE(myBuffer3.appendBits(0xabcdef, 7));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
    EXPECT_EQ(0x6f, buf[0]);
    EXPECT_TRUE(myBuffer3.appendBits(0x1, 1));
    EXPECT_EQ(0xef, buf[0]);
    EXPECT_FALSE(myBuffer3.appendBits(0x8877665544332211, 64));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
    EXPECT_FALSE(myBuffer3.appendBits(0x8877665544332211, 57));
    EXPECT_TRUE(myBuffer3.appendBits(0x8877665544332211, 50));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
    EXPECT_TRUE(myBuffer3.appendBits(-1, 6));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));

    for (size_t i=0; i<=64; ++i) {
        myBuffer3.purgeBuffer();
        EXPECT_TRUE(myBuffer3.appendBits(0x8877665544332211, i));
    }
    myBuffer3.purgeBuffer();
    EXPECT_FALSE(myBuffer3.appendBits(0x8877665544332211, 65));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
}

TEST(VmfAttributeBufferTest, getBit) {
    uint8_t buf[256];
    memset(buf, 0, 256);
    VmfAttributeBuffer myBuffer(buf, 2);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(1));
    myBuffer.resetIterator();
    uint8_t bit = 0;
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_FALSE(myBuffer.getBit(bit));
}


TEST(VmfAttributeBufferTest, getBitReachTheEndOfBuffer) {
    uint8_t buf[256];
    memset(buf, 0, 256);
    VmfAttributeBuffer myBuffer(buf, 1);
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(1));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(0));
    EXPECT_TRUE(myBuffer.appendBit(1));
    myBuffer.resetIterator();
    uint8_t bit = 0;
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x00, bit);
    EXPECT_TRUE(myBuffer.getBit(bit));
    EXPECT_EQ(0x01, bit);
    EXPECT_FALSE(myBuffer.getBit(bit));
}


TEST(VmfAttributeBufferTest, getBits) {
    uint8_t buf[256];
    VmfAttributeBuffer myBuffer3(buf, 8);
    EXPECT_TRUE(myBuffer3.appendBits(0x8877665544332211, 50));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
    EXPECT_TRUE(myBuffer3.appendBits(-1, 6));
    VmfDebug("Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
    myBuffer3.resetIterator();
    uint64_t myRead;
    EXPECT_FALSE(myBuffer3.getBits(myRead, 64));
    EXPECT_TRUE(myBuffer3.getBits(myRead, 56));
    VmfDebug("myRead=", std::hex, myRead);
    myBuffer3.resetIterator();
    EXPECT_TRUE(myBuffer3.getBits(myRead, 6));
    VmfDebug("myRead=", std::hex, myRead);
    EXPECT_TRUE(myBuffer3.getBits(myRead, 8));
    VmfDebug("myRead=", std::hex, myRead);
    EXPECT_TRUE(myBuffer3.getBits(myRead, 13));
    VmfDebug("myRead=", std::hex, myRead);

    VmfAttributeBuffer myBuffer0(buf, 8);
    myBuffer0.purgeBuffer();
    EXPECT_TRUE(myBuffer0.appendBits(123, 7));
    EXPECT_TRUE(myBuffer0.appendBits(32762, 15));
    EXPECT_TRUE(myBuffer0.appendBits(2147483647, 31));
    VmfDebug("123, 32762, 2147483647, Buffer content:\n", hexDataStr(buf, myBuffer3.getContentSize()));
    myBuffer0.resetIterator();
    EXPECT_TRUE(myBuffer0.getBits(myRead, 7));
    EXPECT_EQ(123, myRead);
    EXPECT_TRUE(myBuffer0.getBits(myRead, 15));
    EXPECT_EQ(32762, myRead);
    EXPECT_TRUE(myBuffer0.getBits(myRead, 31));
    EXPECT_EQ(2147483647, myRead);

    myBuffer3.purgeBuffer();
    EXPECT_TRUE(myBuffer3.appendBits(0x8877665544332211, 64));
    for (size_t i=0; i<=64; ++i) {
        myBuffer3.resetIterator();
        EXPECT_TRUE(myBuffer3.getBits(myRead, i));
    }
    myBuffer3.resetIterator();
    EXPECT_FALSE(myBuffer3.getBits(myRead, 65));
}
