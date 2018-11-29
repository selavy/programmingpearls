// 2. How would you implement bit vectors using bitwise logical operations (such as and, or and shift)?
#include <iostream>
#include <cstdint>
#include <cassert>


class BitVector {
public:
    static const size_t MAX_VALUE = 1 << 20;
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

    constexpr bool isset(size_t index) const noexcept {
        return bits_ & mask(index);
    }

private:
    constexpr bit_type mask(size_t index) const noexcept {
        return static_cast<bit_type>(1) << 1;
    }

private:
    bit_type bits_;
};

int main(int argc, char** argv) {
    BitVector bv;

    assert(bv.isset(2) == false);
    bv.set(2);
    assert(bv.isset(2) == true );

    return 0;
}
