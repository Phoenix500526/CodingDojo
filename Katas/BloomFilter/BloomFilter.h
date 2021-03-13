#pragma once
#include <cmath>
#include <cstdint>
#include <deque>
#include <ratio>

template <std::uint64_t N, class Ratio>
class DefaultOptimalPolicy {
public:
    // 计算出 m 值
    static inline std::uint64_t get_m(std::uint64_t n) {
        return LOG_2_E * static_cast<double>(n) *
               (log2(Ratio::den) - log2(Ratio::num));
    }

    static inline std::uint16_t get_k() {
        return ceil(log2(Ratio::den) - log2(Ratio::num));
    }

private:
    static constexpr double LOG_2_E = 1.442696;
};

template <
    std::uint64_t N, class FalsePositiveProbability = std::ratio<1, 10'000>,
    class OptimalPolicy = DefaultOptimalPolicy<N, FalsePositiveProbability>>
class BloomFilter_t {
public:
    BloomFilter_t() : m(OptimalPolicy::get_m(N)), k(OptimalPolicy::get_k()) {}
    ~BloomFilter_t() {}

private:
    const std::uint64_t m;
    const std::uint16_t k;
    std::deque<bool> m_bitmap;
};