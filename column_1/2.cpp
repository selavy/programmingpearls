// 2. How would you implement bit vectors using bitwise logical operations (such as and, or and shift)?
#include <iostream>
#include <cstdint>
#include <cassert>
#include "catch.hpp"


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


template <size_t Bytes>
class Storage {
public:
    constexpr Storage() noexcept : val{0u} {}
    
    constexpr void set(size_t bit) noexcept {
        const size_t idx = bit / 8;
        const size_t off = bit % 8;
        val[idx] |= mask(off);
    }

    [[nodiscard]] constexpr bool isset(size_t bit) noexcept {
        const size_t idx = bit / 8;
        const size_t off = bit % 8;
        return (val[idx] & mask(off)) != 0u;
    }

    constexpr void clear() noexcept {
        for (int i = 0; i < n_bytes; ++i) {
            val[i] = 0u;
        }
    }

    [[nodiscard]] constexpr bool none() noexcept {
        for (int i = 0; i < n_bytes; ++i) {
            if (val[i] != 0) {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] constexpr bool all() noexcept {
        const uint8_t allset = ~uint8_t{0};
        for (int i = 0; i < n_bytes; ++i) {
            if (val[i] != allset) {
                return false;
            }
        }
        return true;
    }

private:
    [[nodiscard]] constexpr uint8_t mask(size_t off) noexcept {
        assert(off < sizeof(uint8_t)*8);
        return uint8_t{1} << off;
    }

    static constexpr size_t n_bytes = Bytes;
    std::array<uint8_t, n_bytes> val;
};

template <class T>
struct StorageBase {
    using value_type = T;
    // static_assert(std::is_unsigned_v<T> == true, "Type must be unsigned int");
    
    constexpr StorageBase() noexcept : val{0} {}
    
    constexpr void set(size_t bit) noexcept {
        val |= mask(bit);
    }
    
    [[nodiscard]] constexpr bool isset(size_t bit) noexcept {
        return (val & mask(bit)) != 0u;
    }
    
    constexpr void clear() noexcept {
        val = 0;
    }

    [[nodiscard]] constexpr bool none() noexcept {
        return val == value_type{0};
    }

    [[nodiscard]] constexpr bool all() noexcept {
        return val == ~value_type{0};
    }
    
private:
    [[nodiscard]] constexpr value_type mask(size_t off) noexcept {
        assert(off < sizeof(value_type)*8);
        return value_type{1} << off;
    }
    
    value_type val;
};

// template <> struct Storage<1> {
// public:
//     constexpr Storage() noexcept : val{0u} {}
//     constexpr void set(size_t bit) noexcept {
//         val |= mask(bit);
//     }
//     [[nodiscard]] constexpr bool isset(size_t bit) noexcept {
//         return (val & mask(bit)) != 0u;
//     }
//     constexpr void clear() noexcept {
//         val = 0u;
//     }
// private:
//     [[nodiscard]] constexpr uint8_t mask(size_t off) noexcept {
//         assert(off < sizeof(val)*8);
//         return uint8_t{0} << off;
//     }
    
//     uint8_t val;
// };

template <> class Storage<1> : StorageBase<uint8_t>  {};
template <> class Storage<2> : StorageBase<uint16_t> {};
template <> class Storage<4> : StorageBase<uint32_t> {};
template <> class Storage<8> : StorageBase<uint64_t> {};

// TODO(peter): if MaxIndex < 64, can I use a uint64_t instead?
// TODO(peter): should be able to figure out MaxIndex given MaxValue
template <size_t MaxIndex>
class BitVector2 : public Storage<MaxIndex / 8> {};


TEST_CASE("Set and clear bits", "[bitvector]") {
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

TEST_CASE("Set and clear bits for non-specialized BitVector2", "[bitvector2]") {
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

