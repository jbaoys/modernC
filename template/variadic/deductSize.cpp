#include <cstddef>
#include <cstdint>
#include <array>
#include <iostream>
#include <type_traits> // For std::is_same_v
#include <algorithm>    // For std::copy
#include <iterator>     // For std::begin, std::end

struct MyStruct {
    std::array<int, 3> data; // Explicitly defining size for demonstration
};

template<size_t N>
struct MyStructDeduce {
    const int data[N]; // No size specified here, it will be deduced
};

template<size_t N>
struct portCfg
{
    uint8_t port = 0;
    uint8_t localAddresses[N];
    portCfg(const long unsigned int p, const long unsigned int (&a)[N]) : port(p) {
        std::copy(std::begin(a), std::end(a), std::begin(localAddresses));
    } 
};
template<size_t N>
void showportCfg( portCfg<N> arr, const char* name ) {
    printf("%s.port = 0x%02X\n", name, arr.port);
    for (int i=0; i < N; ++i) printf("%s.localAddresses[%i] = 0x%02X\n", name, i, arr.localAddresses[i]);
}


template<typename T, size_t N>
constexpr size_t COUNT_OF(T (&)[N]) noexcept { return N; };

struct Cfg {
    uint8_t port = 0;
    uint8_t logicalAddresses[5];
    bool internalLoopback = false;
    size_t num = 0;
    template<size_t N>
    Cfg(const long unsigned int p, const long unsigned int (&a)[N], const bool lpbk) : port(p), internalLoopback(lpbk), num(N) {
        std::copy(std::begin(a), std::end(a), std::begin(logicalAddresses));
    }
};

template<size_t N>
void showCfgTable(const Cfg (&table)[N]) {
    for (int i=0; i<N; ++i) {
        printf("Physical port<%i> config --> [0x%02X] <", i, table[i].port);
        for (int j=0; j<table[i].num; ++j )
            printf("0x%02X ", table[i].logicalAddresses[j]);
        printf("> %s\n", table[i].internalLoopback ? "Yes" : "No");
    }
}

const Cfg spwRCfgTable[] = {
    { 1, { 0x21, 0x22 }, false },
    { 2, { 0x32 }, true },
    { 3, { 0x33, 0x45, 0xFE, 0x37 }, true },
};

int main() {
    // Explicitly sized std::array in a struct
    MyStruct s1 = {{1, 2, 3}};
    std::cout << "Size of s1.data: " << s1.data.size() << std::endl; // Output: 3

    // Size deduced std::array in a struct using C++17 deduction guides
    //MyStructDeduce s2 = {10, 20, 30, 40}; // Initializer list drives deduction
    //std::cout << "Size of s2.data: " << s2.data.size() << std::endl; // Output: 4

    // Verify the type deduction (optional)
    //static_assert(std::is_same_v<decltype(s2.data), std::array<int, 4>>, "Type deduction failed");

    portCfg foo = {1, { 9, 8, 7, 6, 5, 4, 3,2,1 }};
    showportCfg(foo, "foo");
    portCfg other = {7, { 100, 99, 201 }};
    showportCfg(other, "other");

    showCfgTable(spwRCfgTable);
    return 0;
}
