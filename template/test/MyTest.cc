#include <gtest/gtest.h>
#include <string>

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

// Custom companion macro
#define ASSERT_STRUCT_CUSTOM(actual, expected, message, ...) \
    do { \
        SCOPED_TRACE(message); \
        AssertStructsCustom(actual, expected, ##__VA_ARGS__); \
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
