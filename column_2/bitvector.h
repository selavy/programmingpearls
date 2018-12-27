#pragma once

#include <cstdint>
#include <cassert>
#include <array>

namespace col2 {

namespace detail {

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

    // TODO(peter): size_t may not be big enough depending on how many value
    //              are supposed to be supported
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

template <> class Storage<1> : public StorageBase<uint8_t>  {};
template <> class Storage<2> : public StorageBase<uint16_t> {};
template <> class Storage<4> : public StorageBase<uint32_t> {};
template <> class Storage<8> : public StorageBase<uint64_t> {};

}

// TODO(peter): if MaxIndex < 64, can I use a uint64_t instead?
// TODO(peter): should be able to figure out MaxIndex given MaxValue
template <size_t MaxIndex>
class BitVector : public detail::Storage<MaxIndex / 8> {};

}
