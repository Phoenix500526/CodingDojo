#pragma once
#include <cassert>
#include <cstdint>
#include <iostream>
#include <variant>
#include <vector>

namespace detail {
constexpr std::uint8_t TEN_PINS = 10;

enum class bonus_t {
    zero,
    one,
    two,
};

// 状态机的起点和终点
struct game_start {
    inline bool operator==(game_start rhs) const { return true; }
};
struct game_over {
    inline bool operator==(game_over rhs) const { return true; }
};

// 普通得分阶段
struct strike {
    inline bool operator==(strike rhs) const { return true; }
};
struct spare {
    spare(std::uint8_t fir, std::uint8_t sec)
        : first_score(fir), second_score(sec) {
        assert(first_score + second_score == 10);
    }
    inline bool operator==(spare rhs) const {
        return first_score == rhs.first_score &&
               second_score == rhs.second_score;
    }
    std::uint8_t first_score;
    std::uint8_t second_score;
};
struct miss {
    miss(std::uint8_t fir = 0, std::uint8_t sec = 0)
        : first_score(fir), second_score(sec) {
        assert((first_score < 10) && (second_score < 10) &&
               (first_score + second_score < 10));
    }
    inline bool operator==(miss rhs) const {
        return first_score == rhs.first_score &&
               second_score == rhs.second_score;
    }
    std::uint8_t first_score;
    std::uint8_t second_score;
};

// 加时清算阶段
struct clear {
    inline bool operator==(clear rhs) const { return true; }
};

template <typename... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}  // namespace detail

class bowling_t {
private:
    using state_t =
        std::variant<detail::game_start, detail::strike, detail::spare,
                     detail::miss, detail::clear, detail::game_over>;
    state_t m_state;
    std::vector<detail::bonus_t> m_bonus;
    const int m_maxFrame;
    int m_frameID;
    int m_score;

    inline void clear_bonus(int frameID, std::uint8_t score) {
        using namespace detail;
        for (int i = 0; i < 2; ++i) {
            --frameID;
            if (frameID <= 0) {
                break;
            }
            switch (m_bonus[frameID]) {
                case bonus_t::zero:
                    break;
                case bonus_t::one:
                    m_score += score;
                    m_bonus[frameID] = bonus_t::zero;
                    break;
                case bonus_t::two:
                    m_score += score;
                    m_bonus[frameID] = bonus_t::one;
                    break;
            }
        }
    }

public:
    bowling_t(std::uint8_t frames = 10)
        : m_state(detail::game_start()),
          m_bonus(frames + 3, detail::bonus_t::zero),
          m_maxFrame(frames),
          m_frameID(0),
          m_score(0) {}
    ~bowling_t() = default;
    auto get_state() const { return m_state; }

    auto get_score() const { return m_score; }

    auto get_frameID() const { return m_frameID; }

    auto get_bonus() const { return m_bonus[m_frameID]; }

    auto frame_for(const state_t& result) {
        ++m_frameID;
        std::visit(
            detail::overloaded{
                [&](detail::game_start state) {
                    using namespace detail;
                    if (std::holds_alternative<miss>(result)) {
                        auto temp = std::get<miss>(result);
                        m_score += temp.first_score + temp.second_score;
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                    } else if (std::holds_alternative<strike>(result)) {
                        m_score += TEN_PINS;
                        m_bonus[m_frameID] = bonus_t::two;
                        clear_bonus(m_frameID, TEN_PINS);
                    } else {
                        auto temp = std::get<spare>(result);
                        m_score += TEN_PINS;
                        m_bonus[m_frameID] = bonus_t::one;
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                    }
                    m_state = m_frameID < m_maxFrame ? result : clear{};
                },
                [&](const detail::strike& state) {
                    using namespace detail;
                    if (std::holds_alternative<miss>(result)) {
                        auto temp = std::get<miss>(result);
                        m_score += temp.first_score + temp.second_score;
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                        m_state = m_frameID < m_maxFrame ? result : game_over{};
                    } else if (std::holds_alternative<strike>(result)) {
                        m_score += TEN_PINS;
                        m_bonus[m_frameID] = bonus_t::two;
                        clear_bonus(m_frameID, TEN_PINS);
                        m_state = m_frameID < m_maxFrame ? result : clear{};
                    } else {
                        auto temp = std::get<spare>(result);
                        m_score += TEN_PINS;
                        m_bonus[m_frameID] = bonus_t::one;
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                        m_state = m_frameID < m_maxFrame ? result : clear{};
                    }
                },
                [&](const detail::spare& state) {
                    using namespace detail;
                    if (std::holds_alternative<miss>(result)) {
                        auto temp = std::get<miss>(result);
                        m_score += temp.first_score + temp.second_score;
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                        m_state = m_frameID < m_maxFrame ? result : game_over{};
                    } else if (std::holds_alternative<strike>(result)) {
                        m_score += TEN_PINS;
                        m_bonus[m_frameID] = bonus_t::two;
                        clear_bonus(m_frameID, TEN_PINS);
                        m_state = m_frameID < m_maxFrame ? result : clear{};
                    } else {
                        auto temp = std::get<spare>(result);
                        m_score += TEN_PINS;
                        m_bonus[m_frameID] = bonus_t::one;
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                        m_state = m_frameID < m_maxFrame ? result : clear{};
                    }
                },
                [&](const detail::miss& state) {
                    using namespace detail;
                    if (std::holds_alternative<miss>(result)) {
                        auto temp = std::get<miss>(result);
                        m_score += temp.first_score + temp.second_score;
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                        m_state = m_frameID < m_maxFrame ? result : game_over{};
                    } else if (std::holds_alternative<strike>(result)) {
                        m_score += TEN_PINS;
                        m_bonus[m_frameID] = bonus_t::two;
                        clear_bonus(m_frameID, TEN_PINS);
                        m_state = m_frameID < m_maxFrame ? result : clear{};
                    } else {
                        auto temp = std::get<spare>(result);
                        m_score += TEN_PINS;
                        m_bonus[m_frameID] = bonus_t::one;
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                        m_state = m_frameID < m_maxFrame ? result : clear{};
                    }
                },
                [&](const detail::clear& state) {
                    using namespace detail;
                    if (std::holds_alternative<miss>(result)) {
                        auto temp = std::get<miss>(result);
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                        m_state = game_over{};
                    } else if (std::holds_alternative<strike>(result)) {
                        clear_bonus(m_frameID, TEN_PINS);
                        if (m_frameID == m_maxFrame + 2) {
                            m_state = game_over{};
                        }
                    } else {
                        auto temp = std::get<spare>(result);
                        clear_bonus(m_frameID, temp.first_score);
                        clear_bonus(m_frameID, temp.second_score);
                        m_state = game_over{};
                    }
                    if (m_bonus[m_maxFrame] == bonus_t::zero) {
                        m_state = game_over{};
                    } else {
                        m_state = clear{};
                    }
                },
                [&](const detail::game_over& state) {
                    std::cerr << "The game is end, your score is " << m_score
                              << '\n';
                }},
            m_state);
        return m_state;
    }
};