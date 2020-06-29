#pragma once

#include <cstdint>
#include <cstring>

namespace util
{
    constexpr uint32_t fnv1a_seed = 0x37C4271F;
    constexpr uint32_t fnv1a_prime = 0x1000193;
    __declspec(dllexport) extern uint32_t runtime_basis;

    inline uint32_t fnv1a(const char* key) {
        const char* data = const_cast<char*>(key);
        auto hash = runtime_basis;

        for (size_t i = 0; i < strlen(key); ++i) {
            const uint8_t value = data[i];
            hash = hash ^ value;
            hash *= fnv1a_prime;
        }

        return hash;
    }

    constexpr uint32_t fnv1a_ct(const char* str, const uint32_t value = fnv1a_seed) noexcept {
        return !*str ? value : fnv1a_ct(str + 1, static_cast<unsigned>(1ull *
                                                                       (value ^ static_cast<uint8_t>(*str)) * fnv1a_prime));
    }
}

template<typename t, t v>
struct constant_holder
{
    enum class val_holder : t
    {
        val = v
    };
};

#define CONSTANT(value) ((decltype(value))constant_holder<decltype(value), value>::val_holder::val)

#define fnv(s) (CONSTANT(util::fnv1a_ct(s)))
#define fnv_cmp(a, b) (CONSTANT(util::fnv1a_ct(b)) == util::fnv1a(a))
#define fnv_cmp_imp(a, b) (b == util::fnv1a(a))