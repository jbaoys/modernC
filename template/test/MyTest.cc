#include <gtest/gtest.h>
#include <string>
#include <functional> //std::
#include <iostream>
#include <type_traits>

#include "Event.h"

// 1. The Compile-Time Detector Trait
template <typename T, typename = std::void_t<>>
struct has_equality_operator : std::false_type {};

// This specialization matches ONLY if 'a == b' is a valid expression for type T
template <typename T>
struct has_equality_operator<T, std::void_t<decltype(std::declval<T>() == std::declval<T>())>>
    : std::true_type {};

// Helper shortcut variable (C++17 style)
template <typename T>
inline constexpr bool has_equality_operator_v = has_equality_operator<T>::value;



// Example generic structs
struct Point {
    int x;
    int y;
};

struct User {
    int id;
    std::string name;
};

// Sample C++17 Structs
struct Employee {
    int id;
    std::string name;
};

struct Product {
    int sku;
    double price;
};

// 1. Define the generic gtest sub-procedure using a template
template <typename T>
void AssertStructsAreEqual(const T& actual, const T& expected, const std::string& message = "") {
    // Use the member-by-member comparison logic.
    // For generic types without overloaded operator==, you can use C++20 designated initializers
    // or structured binding if you know the maximum field count, 
    // but the cleanest C++20 way is overloading operator== or using a custom comparator lambda.
    EXPECT_EQ(actual, expected);
}

// Custom companion macro
#define ASSERT_STRUCT_FIELD(actual, expected, message) \
    do { \
        SCOPED_TRACE(message); \
        AssertStructsAreEqual(actual, expected, #message); \
    } while (0)


// Generic Sub-procedure accepting a custom comparator
template <typename T1, typename T2, typename Comparator = std::equal_to<T1>>
void AssertStructsCustom(const T1& actual, const T2& expected, Comparator comp = Comparator{}) {
    // Evaluated strictly as a regular runtime boolean condition
    bool types_are_identical = std::is_same_v<T1, T2>; 

    if (types_are_identical) {
        // Types match! We can safely invoke the custom or default comparator at runtime.
        // We use an internal reinterpret_cast trick just to satisfy the compiler's type checker 
        // in case T1 and T2 are different (even though this branch never executes if they differ).
        auto& safe_expected = reinterpret_cast<const T1&>(expected);
        bool match = comp(actual, safe_expected);
        EXPECT_TRUE(match) << "Struct fields do not match!";
    } else {
        // Types mismatch! Evaluated dynamically at runtime.
        FAIL() << "Runtime Type Mismatch Error!\n"
               << "  Actual type:   " << typeid(T1).name() << "\n"
               << "  Expected type: " << typeid(T2).name() << "\n";
    }
}

/*
#include <iostream>
#include <functional>

struct Pkt {
    int id;
    int priority;
};

// 1. Define a clean alias for your comparison function pointer signature.
// This function takes two packets and returns true if the first is "less" than the second.
using PktCompareFunc = bool(*)(const Pkt&, const Pkt&);

// 2. The Template Function
template <typename T>
bool comparePackets(const T& a, const T& b, bool(*comp)(const T&, const T&) = nullptr) {
    // Fallback: If no custom comparison function is provided, use the default operator<
    if (comp == nullptr) {
        return a.priority < b.priority; 
    }
    
    // Custom logic: Execute the function pointer passed by the user
    return comp(a, b);

#include <functional>

template <typename T>
bool comparePackets(const T& a, const T& b, std::function<bool(const T&, const T&)> comp = nullptr) {
    // Check if the std::function holds a valid lambda or function pointer
    if (!comp) { 
        return a.priority < b.priority; // Fallback
    }
    
    return comp(a, b); // Execute whatever lambda/function was passed
}

// 1. Rewrite your template function to accept an independent 'Compare' type
template <typename T1, typename Compare = std::nullptr_t>
bool comparePackets(const T1& a, const T1& b, Compare comp = nullptr) {

    // 2. Use constexpr check to see if no lambda was provided (it remained nullptr)
    if constexpr (std::is_same_v<Compare, std::nullptr_t>) {
        return a.priority < b.priority; // Fallback logic
    } else {
        // 3. If comp is a std::function or raw pointer, still check for runtime null
        if constexpr (std::is_pointer_v<Compare>) {
            if (comp == nullptr) return a.priority < b.priority;
        }

        return comp(a, b); // Executes your lambda perfectly!
    }
}

}

template <typename T1>
bool comparePackets(const T1& a, const T1& b) {
    // Compile-time check
    if constexpr (has_equality_operator_v<T1>) {
        std::cout << "Compile-time detection: operator== exists!\n";
        return (a == b); 
    } else {
        std::cout << "Compile-time detection: operator== MISSING! Using fallback member comparison.\n";
        return a.priority < b.priority; 
    }
}

 */

template <typename T1, typename T2>
void ExpectEvtEq(const T1& actual, const T2& expected) {
    // Evaluated strictly as a regular runtime boolean condition
    bool types_are_identical = std::is_same_v<T1, T2>; 

    if (types_are_identical) {
        // Types match! We can safely invoke the custom or default comparator at runtime.
        // We use an internal reinterpret_cast trick just to satisfy the compiler's type checker 
        // in case T1 and T2 are different (even though this branch never executes if they differ).
        auto& safe_expected = reinterpret_cast<const T1&>(expected);
        EXPECT_EQ(expected.eventId, actual.eventId) << "Event ID do not match!";
        if constexpr (has_equality_operator_v<T1>) {
            EXPECT_EQ(safe_expected.eventData, actual.eventData) << "Event data do not match!";
        }
    } else {
        // Types mismatch! Evaluated dynamically at runtime.
        FAIL() << "Runtime Type Mismatch Error!\n"
               << "  Actual type:   " << typeid(T1).name() << "\n"
               << "  Expected type: " << typeid(T2).name() << "\n";
    }
}
#define EXPECT_EVT_EQ(actual, expected, message) \
    do { \
        SCOPED_TRACE(message); \
        ExpectEvtEq(actual, expected); \
    } while (0)


template <typename T1, typename T2, typename Compare = std::nullptr_t>
void AssertEventEq(const T1& actual, const T2& expected, Compare comp = nullptr) {
    printf("in AssertEventEq\n");
    // Evaluated strictly as a regular runtime boolean condition
    bool types_are_identical = std::is_same_v<T1, T2>; 

    if (types_are_identical) {
        printf("in types_are_identical\n");
        // Types match! We can safely invoke the custom or default comparator at runtime.
        // We use an internal reinterpret_cast trick just to satisfy the compiler's type checker 
        // in case T1 and T2 are different (even though this branch never executes if they differ).
        auto& safe_expected = reinterpret_cast<const T1&>(expected);
        bool match = true;
        bool fullComp = true;
        if constexpr (std::is_same_v<Compare, std::nullptr_t>) {
            printf("Comp type = nullptr\n");
            fullComp = false;
        } else if constexpr (std::is_pointer_v<Compare>) {
            if (comp == nullptr) {
                printf("Comp = nullptr\n");
                fullComp = false;
            }
        }

        if (fullComp) {
            printf("Comp ! = nullptr\n");
            match = comp(actual, safe_expected);
        }
        EXPECT_TRUE(match) << "Struct fields do not match!";
    } else {
        // Types mismatch! Evaluated dynamically at runtime.
        FAIL() << "Runtime Type Mismatch Error!\n"
               << "  Actual type:   " << typeid(T1).name() << "\n"
               << "  Expected type: " << typeid(T2).name() << "\n";
    }
}

// Custom companion macro
#define ASSERT_STRUCT_CUSTOM(actual, expected, message, ...) \
    do { \
        SCOPED_TRACE(message); \
        AssertStructsCustom(actual, expected, ##__VA_ARGS__); \
    } while (0)

#define ASSERT_EVENT_EQ(actual, expected, message, ...) \
    do { \
        SCOPED_TRACE(message); \
        AssertEventEq(actual, expected, ##__VA_ARGS__); \
    } while (0)


// Compare event
template <typename T1, typename T2, typename Compare = std::nullptr_t>
void ExpectEventEq(const T1& actual, const T2& expected, Compare comp = nullptr) {
    // Evaluated strictly as a regular runtime boolean condition
    bool types_are_identical = std::is_same_v<T1, T2>; 

    if (types_are_identical) {
        // Types match! We can safely invoke the custom or default comparator at runtime.
        // We use an internal reinterpret_cast trick just to satisfy the compiler's type checker 
        // in case T1 and T2 are different (even though this branch never executes if they differ).
        auto& safe_expected = reinterpret_cast<const T1&>(expected);
        EXPECT_EQ(expected.eventId, actual.eventId) << "Event ID do not match!";
        if constexpr (std::is_same_v<Compare, std::nullptr_t>) {
            printf("default nullptr_t\n");
        } else if constexpr (std::is_pointer_v<Compare>) {
            if (comp == nullptr) {
                printf("default nullptr\n");
            } else {
                EXPECT_TRUE(comp(actual.eventData, safe_expected.eventData)) << "Event data do not match!";
            }
        } else {
            EXPECT_TRUE(comp(actual.eventData, safe_expected.eventData)) << "Event data do not match!";
        }
    } else {
        // Types mismatch! Evaluated dynamically at runtime.
        FAIL() << "Runtime Type Mismatch Error!\n"
               << "  Actual type:   " << typeid(T1).name() << "\n"
               << "  Expected type: " << typeid(T2).name() << "\n";
    }
}

#define EXPECT_EVENT_EQ(actual, expected, message, ...) \
    do { \
        SCOPED_TRACE(message); \
        ExpectEventEq(actual, expected, ##__VA_ARGS__); \
    } while (0)

// 2. Custom companion for structures without an operator== (C++20 Default Comparison)
// If your structs use C++20, you can simply add "auto operator<=>(const Point&) const = default;" 
// inside the struct. If you cannot modify the struct, you can overload operator== like this:
bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

bool operator==(const User& lhs, const User& rhs) {
    return lhs.id == rhs.id && lhs.name == rhs.name;
}

bool operator== (const myPkt1& lhs, const myPkt1& rhs) {
    printf("event data comparision \n");
    return lhs.start == rhs.start &&
           lhs.pktId == rhs.pktId &&
           lhs.fcs == rhs.fcs &&
           lhs.end == rhs.end;
}

// 3. Test Cases
TEST(GenericStructTest, ComparePoints) {
    Point p1{10, 20};
    Point p2{10, 20};
    Point p3{15, 20};

    // This will pass
    AssertStructsAreEqual(p1, p2, "Comparing identical Point structs");

    // This will fail, and SCOPED_TRACE will point to this exact line
    ASSERT_STRUCT_FIELD(p1, p3, "Comparing different Point structs");
}

TEST(GenericStructTest, CompareUsers) {
    User u1{1, "Alice"};
    User u2{1, "Alice"};

    AssertStructsAreEqual(u1, u2, "Comparing identical User structs");
}


TEST(Cpp17Test, LambdaComparison) {
    Product p1{5501, 19.99};
    Product p2{5501, 19.99};

    // Pass the comparison logic inline
    ASSERT_STRUCT_CUSTOM(p1, p2, "Testing products via lambda", [](const Product& a, const Product& b) {
        return a.sku == b.sku && a.price == b.price;
    });
}

TEST(Cpp17Test, LambdaComparison_default) {
    Point p1{10, 20};
    Point p2{15, 20};

    ASSERT_STRUCT_CUSTOM(p1, p2, "p1 != p2");
}

TEST(Cpp17Test, LambdaComparison_wrongTypes) {
    Point p1{10, 20};
    Product p2{5501, 19.99};

    ASSERT_STRUCT_CUSTOM(p1, p2, "p1 != p2");
}

TEST(Cpp17Test, LambdaComparison_default2) {
    Employee e1{1101, "John Hunts"};
    Employee e2{1101, "Tom Smith"};

    ASSERT_STRUCT_CUSTOM(e1, e2, "e1 != e2", [](const Employee&a, const Employee&b) {
            return a.id == b.id;});
}

TEST(EventTest, normal_assert) {
    MyEvent1 event1 = MY_EVENT1;
    MyEvent1 EXPECT_EVENT1 = {0x124, {0x7e, 0xbeef1010, 0, 0x7e}};
    printf("Start EventTest.normal\n");
    ASSERT_EVENT_EQ(EXPECT_EVENT1, event1, "EXPECT_EVENT1 != event1", [](const MyEvent1& a, const MyEvent1& b) {
                printf("hit this\n");
                return a.eventId == b.eventId;
            });
}

TEST(EventTest, error_expect) {
    MyEvent1 event1 = MY_EVENT1;
    MyEvent1 EXPECT_EVENT1 = {121, {0x7e, 0xbeef1010, 0, 0x7e}};
    EXPECT_EVENT_EQ(EXPECT_EVENT1, event1, "EXPECT_EVENT1 != event1");
}

TEST(EventTest, normal_expect) {
    MyEvent1 event1 = MY_EVENT1;
    MyEvent1 EXPECT_EVENT1 = {123, {0x7f, 0xbeef1010, 0, 0x7e}};
    EXPECT_EVENT_EQ(EXPECT_EVENT1, event1, "EXPECT_EVENT1 != event1");
}

TEST(EventTest, error_expect_full) {
    MyEvent1 event1 = MY_EVENT1;
    MyEvent1 EXPECT_EVENT1 = {123, {0x7e, 0xbeef1010, 2, 0x7e}};
    EXPECT_EVENT_EQ(EXPECT_EVENT1, event1, "EXPECT_EVENT1 != event1", [](const myPkt1& a, const myPkt1& b) {
                printf("event data comparision \n");
                return a.start == b.start &&
                       a.pktId == b.pktId &&
                       a.fcs == b.fcs &&
                       a.end == b.end;
            });
}

TEST(EventTest, normal_expect_full) {
    MyEvent1 event1 = MY_EVENT1;
    MyEvent1 EXPECT_EVENT1 = {123, {}};
    EXPECT_EVENT_EQ(EXPECT_EVENT1, event1, "EXPECT_EVENT1 != event1", [](const myPkt1& a, const myPkt1& b) {
                printf("event data comparision \n");
                return a.start == b.start &&
                       a.pktId == b.pktId &&
                       a.fcs == b.fcs &&
                       a.end == b.end;
            });
}

TEST(EventTest, normal_expect_with_override_eq) {
    MyEvent1 event1 = MY_EVENT1;
    MyEvent1 EXPECT_EVENT1 = {123, {}};
    EXPECT_EVT_EQ(EXPECT_EVENT1, event1, "EXPECT_EVENT1 != event1");
}

TEST(EventTest, normal_expect_with_override_eq2) {
    MyEvent2 event2 = MY_EVENT2;
    MyEvent2 EXPECT_EVENT2 = {122, {}};
    EXPECT_EVT_EQ(EXPECT_EVENT2, event2, "EXPECT_EVENT2 != event2");
}
