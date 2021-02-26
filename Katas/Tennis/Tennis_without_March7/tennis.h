#pragma once

#include <variant>
namespace tennis {
namespace detail {

// 基本的分数情况，对于有特殊意义的得分，分离为 forty 的情况
enum class points {
    love,
    fifteen,
    thirty,
};

enum class player {
    player_1,
    player_2,
};

struct normal_scoring {
    points player_1_points;
    points player_2_points;
    inline bool operator==(const normal_scoring& other) const {
        return player_1_points == other.player_1_points &&
               player_2_points == other.player_2_points;
    }
};

struct forty_scoring {
    player leading_player;
    points other_player_points;
    inline bool operator==(const forty_scoring& other) const {
        return leading_player == other.leading_player &&
               other_player_points == other.other_player_points;
    }
};

struct deuce {};

struct advantage {
    player player_with_advantage;
    inline bool operator==(const advantage& other) const {
        return player_with_advantage == other.player_with_advantage;
    }
};

struct gameover {
    player winner;
    inline bool operator==(const gameover& other) const {
        return winner == other.winner;
    }
};

template <typename E, E first, E head>
void advanceEnum(E& v) {
    if (v == head) v = first;
}

template <typename E, E first, E head, E next, E... tail>
void advanceEnum(E& v) {
    if (v == head)
        v = next;
    else
        advanceEnum<E, first, next, tail...>(v);
}

template <typename E, E first, E... values>
struct EnumValues {
    static void advance(E& v) { advanceEnum<E, first, first, values...>(v); }
};

using points_values =
    EnumValues<points, points::love, points::fifteen, points::thirty>;

points& operator++(points& f) {
    points_values::advance(f);
    return f;
}

template <typename... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}  // namespace detail

class tennis_t {
public:
    tennis_t()
        : m_state(detail::normal_scoring{detail::points::love,
                                         detail::points::love}) {}
    auto get_state() const { return m_state; }
    auto point_for(detail::player which_player) {
        std::visit(
            detail::overloaded{
                [&](detail::normal_scoring& state) {
                    using namespace detail;
                    switch (which_player) {
                        case player::player_1:
                            if (state.player_1_points != points::thirty) {
                                ++state.player_1_points;
                            } else {
                                m_state = forty_scoring{player::player_1,
                                                        state.player_2_points};
                            }
                            break;
                        case player::player_2:
                            if (state.player_2_points != points::thirty) {
                                ++state.player_2_points;
                            } else {
                                m_state = forty_scoring{player::player_2,
                                                        state.player_1_points};
                            }
                            break;
                    }
                },
                [&](detail::forty_scoring& state) {
                    using namespace detail;
                    switch (which_player) {
                        case player::player_1:
                            if (state.leading_player != player::player_1) {
                                if (state.other_player_points ==
                                    points::thirty) {
                                    m_state = deuce{};
                                } else {
                                    ++state.other_player_points;
                                }
                            } else {
                                m_state = gameover{state.leading_player};
                            }
                            break;
                        case player::player_2:
                            if (state.leading_player != player::player_2) {
                                if (state.other_player_points ==
                                    points::thirty) {
                                    m_state = deuce{};
                                } else {
                                    ++state.other_player_points;
                                }

                            } else {
                                m_state = gameover{state.leading_player};
                            }
                            break;
                    }
                },
                [&](detail::deuce& state) {
                    using namespace detail;
                    m_state = advantage{which_player};
                },
                [&](detail::advantage& state) {
                    using namespace detail;
                    switch (which_player) {
                        case player::player_1:
                            if (state.player_with_advantage !=
                                player::player_1) {
                                m_state = deuce{};
                            } else {
                                m_state = gameover{state.player_with_advantage};
                            }
                            break;
                        case player::player_2:
                            if (state.player_with_advantage !=
                                player::player_2) {
                                m_state = deuce{};
                            } else {
                                m_state = gameover{state.player_with_advantage};
                            }
                            break;
                    }
                },
                [&](detail::gameover& state) { using namespace detail; },
            },
            m_state);
        return get_state();
    }

private:
    std::variant<detail::normal_scoring, detail::forty_scoring, detail::deuce,
                 detail::advantage, detail::gameover>
        m_state;
};

}  // namespace tennis