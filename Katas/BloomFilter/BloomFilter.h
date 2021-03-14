#pragma once
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <random>
#include <ratio>
#include <string_view>
#include <vector>

template <std::uint64_t N, class Ratio>
class DefaultOptimalPolicy {
public:
    // 计算出 m 值
    static inline std::uint64_t get_bitmapSize(std::uint64_t n) {
        return LOG_2_E * static_cast<double>(n) *
               (log2(Ratio::den) - log2(Ratio::num));
    }

    static inline std::uint16_t get_numsOfHash() {
        return ceil(log2(Ratio::den) - log2(Ratio::num));
    }

private:
    static constexpr double LOG_2_E = 1.442696;
};

class DefaultHashPolicy {
public:
    using hash_type = std::uint32_t;
    hash_type operator()(std::string_view key, hash_type hash) {
        std::size_t key_len = key.size();
        auto begin = key.begin();
        std::size_t loop = 0;
        while (key_len >= 8) {
            const hash_type i1 =
                *(reinterpret_cast<const hash_type*>(&(*begin)));
            begin += sizeof(i1);
            const hash_type i2 =
                *(reinterpret_cast<const hash_type*>(&(*begin)));
            begin += sizeof(i2);

            hash ^= (hash << 7) ^ i1 * (hash >> 3) ^
                    (~((hash << 11) + (i2 ^ (hash >> 5))));

            key_len -= (sizeof(i1) + sizeof(i2));
        }

        if (key_len) {
            if (key_len >= 4) {
                const hash_type i =
                    *(reinterpret_cast<const hash_type*>(&(*begin)));
                hash ^= (hash << 7) ^ i * (hash >> 3);
                ++loop;
                key_len -= sizeof(i);
                begin += sizeof(i);
            }
            if (key_len >= 2) {
                const std::uint16_t i =
                    *(reinterpret_cast<const std::uint16_t*>(&(*begin)));
                if (loop & 0x01)
                    hash ^= (~((hash << 11) + (i ^ (hash >> 5))));
                else
                    hash ^= (hash << 7) ^ i * (hash >> 3);
                ++loop;
                key_len -= sizeof(i);
                begin += sizeof(i);
            }
            if (key_len) {
                hash += ((*begin) ^ (hash * 0xDEADBEEF)) + loop;
            }
        }
        return hash;
    }
};

template <
    std::uint64_t N, class FalsePositiveProbability = std::ratio<1, 10'000>,
    class OptimalPolicy = DefaultOptimalPolicy<N, FalsePositiveProbability>,
    class HashPolicy = DefaultHashPolicy>
class BloomFilter_t {
public:
    using cell_type = std::uint8_t;
    using bitmap_type = std::vector<cell_type>;
    using hash_type = typename HashPolicy::hash_type;
    using salt_type = std::vector<hash_type>;
    BloomFilter_t()
        : m_insertCount(0),
          m_tableSize(OptimalPolicy::get_bitmapSize(N)),
          m_bitmap(m_tableSize >> 3, 0x00),
          m_salt(OptimalPolicy::get_numsOfHash()),
          m_hash() {
        std::random_device rd;
        std::default_random_engine seed(rd());
        std::uniform_int_distribution<hash_type> distributor(0x1000, 0x7FFF);
        for (auto& item : m_salt) {
            auto temp = distributor(seed) * (distributor(seed) * 4 + 3);
            if (temp == 0 ||
                std::find(m_salt.begin(), m_salt.end(), temp) != m_salt.end()) {
                continue;
            }
            item = temp;
        }
    }
    ~BloomFilter_t() {}

    inline void clear() {
        std::fill(m_bitmap.begin(), m_bitmap.end(), 0x00);
        m_insertCount = 0;
    }

    inline auto hash_impl(std::string_view key) {
        std::vector<hash_result_t> hash_results(m_salt.size(),
                                                hash_result_t{0, 0});
        for (int i = 0; i < m_salt.size(); ++i) {
            hash_results[i].bit_index = m_hash(key, m_salt[i]) % m_tableSize;
            hash_results[i].bit = hash_results[i].bit_index % s_bitPerElement;
        }
        return hash_results;
    }

    inline void insert(std::string_view key) {
        auto hash_results = std::move(hash_impl(key));
        for (auto res : hash_results) {
            m_bitmap[res.bit_index / s_bitPerElement] |= s_bitMask[res.bit];
        }
        ++m_insertCount;
    }

    template <
        class InputIterator,
        class = std::enable_if_t<
            std::is_same_v<typename InputIterator::value_type, std::string> ||
                std::is_same_v<
                    std::remove_cv_t<typename InputIterator::value_type>,
                    char*>,
            void> >
    inline void insert(InputIterator begin, InputIterator end) {
        for (auto iter = begin; iter != end; ++iter) {
            insert(*iter);
        }
    }

    inline bool query(std::string_view key) {
        auto hash_results = std::move(hash_impl(key));
        for (auto res : hash_results) {
            if (!(m_bitmap[res.bit_index / s_bitPerElement] &
                  s_bitMask[res.bit])) {
                return false;
            }
        }
        return true;
    }

    template <
        class InputIterator,
        class = std::enable_if_t<
            std::is_same_v<typename InputIterator::value_type, std::string> ||
                std::is_same_v<
                    std::remove_cv_t<typename InputIterator::value_type>,
                    char*>,
            void> >
    inline InputIterator query(InputIterator begin, InputIterator end) {
        for (auto iter = begin; iter != end; ++iter) {
            if (!query(*iter)) {
                return iter;
            }
        }
        return end;
    }

    inline auto get_salt() const { return m_salt; }

    inline auto get_insertCount() const { return m_insertCount; }

    inline auto get_bitmapSize() const { return m_bitmap.size(); }

    inline auto get_tableSize() const { return m_tableSize; }

private:
    std::uint64_t m_insertCount;
    const std::uint64_t m_tableSize;
    bitmap_type m_bitmap;
    salt_type m_salt;
    HashPolicy m_hash;
    static constexpr std::uint8_t s_bitPerElement = 8;
    static constexpr std::uint8_t s_bitMask[s_bitPerElement] = {
        0x01,  // 00000001
        0x02,  // 00000010
        0x04,  // 00000100
        0x08,  // 00001000
        0x10,  // 00010000
        0x20,  // 00100000
        0x40,  // 01000000
        0x80   // 10000000
    };
    struct hash_result_t {
        std::uint64_t bit_index;
        std::size_t bit;
    };
};
