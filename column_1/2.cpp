// 2. How would you implement bit vectors using bitwise logical operations (such as and, or and shift)?
#include <iostream>
#include <cstdint>
#include <cassert>
#include "catch.hpp"
#include "bit_vector2.h"


[[nodiscard]]
constexpr uint32_t make_mask(size_t bit) noexcept
{
    assert(bit < 32);
    return static_cast<uint32_t>(1u) << bit;
}

[[nodiscard]]
constexpr uint32_t setbit(uint32_t bitfield, size_t bit) noexcept
{
    return bitfield | make_mask(bit);
}

[[nodiscard]]
constexpr uint32_t clearbit(uint32_t bitfield, size_t bit) noexcept
{
    return bitfield & ~make_mask(bit);
}

[[nodiscard]]
constexpr bool isset(uint32_t bitfield, size_t bit) noexcept
{
    return (bitfield & make_mask(bit)) != 0;
}

TEST_CASE("C-style set and clear bits", "[column1_2]")
{
    uint32_t bf = 0u;
    for (int i = 0; i < 32; ++i) {
        REQUIRE(isset(bf, i) == false);
    }
    bf = setbit(bf, 0);
    REQUIRE(bf == 0x01u);
    REQUIRE(isset(bf, 0) == true);

    bf = clearbit(bf, 0);
    REQUIRE(bf == 0x00u);
    REQUIRE(isset(bf, 0) == false);

    for (int i = 0; i < 32; ++i) {
        REQUIRE(isset(bf, i) == false);
        bf = setbit(bf, i);
        REQUIRE(isset(bf, i) == true);
        bf = clearbit(bf, i);
        REQUIRE(isset(bf, i) == false);
    }

    bf = setbit(bf, 31);
    bf = setbit(bf, 30);
    bf = setbit(bf, 29);
    bf = setbit(bf, 28);
    REQUIRE(bf == 0xF0000000u);
    REQUIRE(isset(bf, 31) == true);
    REQUIRE(isset(bf, 30) == true);
    REQUIRE(isset(bf, 29) == true);
    REQUIRE(isset(bf, 28) == true);
    REQUIRE(isset(bf,  0) == false);
    REQUIRE(isset(bf, 15) == false);
    REQUIRE(isset(bf, 20) == false);

    bf = 0;
    for (int i = 0; i < 32; i += 2) {
        bf = setbit(bf, i);
    }
    for (int i = 0; i < 32; ++i) {
        if (i & 1) {
            REQUIRE(isset(bf, i) == false);
        } else {
            REQUIRE(isset(bf, i) == true);
        }
    }
}

TEST_CASE("Set and clear bits first BitVector", "[column1_2]") {
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

