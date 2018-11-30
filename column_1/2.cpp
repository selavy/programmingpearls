// 2. How would you implement bit vectors using bitwise logical operations (such as and, or and shift)?
#include <iostream>
#include <cstdint>
#include <cassert>
#include "catch.hpp"
#include "bit_vector2.h"


class BitVector {
public:
    static constexpr size_t MAX_VALUE = 1 << 20;
    using bit_type = uint32_t;

public:
    constexpr BitVector() noexcept
        : bits_{0} {}

    constexpr void set(size_t index) noexcept {
        bits_ |= mask(index);
    }

    constexpr void clear(size_t index) noexcept {
        bits_ &= ~mask(index);
    }

    constexpr void clear_all() noexcept {
        bits_ = 0;
    }

    [[nodiscard]] constexpr bool isset(size_t index) const noexcept {
        return (bits_ & mask(index)) != 0u;
        // return (bits_ & mask(index)) == mask(index);
    }

    [[nodiscard]] constexpr bool all() const noexcept {
        return bits_ == ~bit_type{0};
    }

    [[nodiscard]] constexpr bool none() const noexcept {
        return bits_ == bit_type{0};
    }

private:
    constexpr bit_type mask(size_t index) const noexcept {
        assert(sizeof(bit_type)*8 > index);
        return static_cast<bit_type>(1) << index;
    }

private:
    bit_type bits_;
};


TEST_CASE("Set and clear bits first BitVector", "[column1_2]") {
    // Storage<8> s8;
    // Storage<1> s1;
    
    BitVector bv;
    REQUIRE(bv.isset(0) == false);
    REQUIRE(bv.isset(1) == false);
    REQUIRE(bv.isset(3) == false);
    REQUIRE(bv.isset(31) == false);
    REQUIRE(bv.isset(21) == false);
    REQUIRE(bv.isset(15) == false);
    REQUIRE(bv.none() == true);
    REQUIRE(bv.all() == false);

    bv.set(4);
    assert(bv.isset(4) == true);
    assert(bv.none() == false);
    assert(bv.all() == false);
    assert(bv.isset(3) == false);
    assert(bv.isset(2) == false);
    assert(bv.isset(1) == false);

    bv.set(31);
    assert(bv.isset(4) == true);
    assert(bv.isset(3) == false);
    assert(bv.isset(31) == true);
    assert(bv.none() == false);
    assert(bv.all() == false);

    for (int i = 0; i < 32; ++i) {
        bv.set(i);
        REQUIRE(bv.isset(i) == true);
    }
    REQUIRE(bv.all() == true);
    REQUIRE(bv.none() == false);
}

TEST_CASE("Set and clear bits for non-specialized BitVector2", "[column1_2]") {
    BitVector2<128> bv;
    REQUIRE(bv.isset(0) == false);
    REQUIRE(bv.isset(1) == false);
    REQUIRE(bv.isset(3) == false);
    REQUIRE(bv.isset(31) == false);
    REQUIRE(bv.isset(21) == false);
    REQUIRE(bv.isset(15) == false);
    REQUIRE(bv.none() == true);
    REQUIRE(bv.all() == false);

    bv.set(4);
    assert(bv.isset(4) == true);
    assert(bv.none() == false);
    assert(bv.all() == false);
    assert(bv.isset(3) == false);
    assert(bv.isset(2) == false);
    assert(bv.isset(1) == false);

    bv.set(31);
    assert(bv.isset(4) == true);
    assert(bv.isset(3) == false);
    assert(bv.isset(31) == true);
    assert(bv.none() == false);
    assert(bv.all() == false);

    for (int i = 0; i < 128; ++i) {
        bv.set(i);
        REQUIRE(bv.isset(i) == true);
    }
    REQUIRE(bv.all() == true);
    REQUIRE(bv.none() == false);
}

TEST_CASE("Set and clear bits for BitVector<32>", "[column1_2]") {
    BitVector2<32> bv;
    REQUIRE(bv.isset(0) == false);
    REQUIRE(bv.isset(1) == false);
    REQUIRE(bv.isset(3) == false);
    REQUIRE(bv.isset(31) == false);
    REQUIRE(bv.isset(21) == false);
    REQUIRE(bv.isset(15) == false);
    REQUIRE(bv.none() == true);
    REQUIRE(bv.all() == false);

    bv.set(4);
    assert(bv.isset(4) == true);
    assert(bv.none() == false);
    assert(bv.all() == false);
    assert(bv.isset(3) == false);
    assert(bv.isset(2) == false);
    assert(bv.isset(1) == false);

    bv.set(31);
    assert(bv.isset(4) == true);
    assert(bv.isset(3) == false);
    assert(bv.isset(31) == true);
    assert(bv.none() == false);
    assert(bv.all() == false);

    for (int i = 0; i < 32; ++i) {
        bv.set(i);
        REQUIRE(bv.isset(i) == true);
    }
    REQUIRE(bv.all() == true);
    REQUIRE(bv.none() == false);
}

