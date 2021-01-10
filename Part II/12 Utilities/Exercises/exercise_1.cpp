#include <optional>
#include <stdexcept>
#include <iostream>
#include "gtest/gtest.h"

template <typename To, typename From>
std::optional<To> narrow_cast(From value) {
    const auto converted = static_cast<To>(value);
    const auto backwards = static_cast<From>(converted);
    if (value != backwards) return std::optional<To>{};
    return std::optional{converted};
}

TEST(NarrowTest, NoOverflow) {
    int short_number{532};
    std::optional<short> narrow = narrow_cast<short, int>(short_number);
    ASSERT_TRUE(narrow.has_value());
}

TEST(NarrowTest, Overflow) {
    int short_number{12323532};
    std::optional<short> narrow = narrow_cast<short, int>(short_number);
    ASSERT_FALSE(narrow.has_value());
}