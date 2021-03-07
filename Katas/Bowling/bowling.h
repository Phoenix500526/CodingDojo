#pragma once
#include <variant>
#include <cstdint>
#include <cassert>

namespace detail{
constexpr std::uint8_t ten_pins = 10;
enum class bonus_t
{
	zero,
	one,
	two,
};

struct game_start{
	/*const*/ bonus_t bonus = bonus_t::zero;
};
struct game_over{
	bonus_t bonus = bonus_t::zero;
};

struct strike
{
	bonus_t bonus = bonus_t::two;
};

struct spare
{
	bonus_t bonus = bonus_t::one;
};

struct miss{
	miss(std::uint8_t fir = 0, std::uint8_t sec = 0):first_point(fir), second_point(sec){
		assert((first_point < 10) && (second_point < 10) && (first_point + second_point < 10));
	}
	std::uint8_t first_point;
	std::uint8_t second_point;
	bonus_t bonus = bonus_t::zero;
};

template <class STATE>
inline bool operator==(STATE lhs, STATE rhs){
	return lhs.bonus == rhs.bonus;
}

template <>
bool operator==<miss>(miss lhs, miss rhs){
	return lhs.first_point == rhs.first_point && lhs.second_point == rhs.second_point && lhs.bonus == rhs.bonus;
}


template <typename... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};

template <typename... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

}	// namespace detail


class bowling_t
{
private:
	using state_t = std::variant<detail::game_start, detail::strike, detail::spare, detail::miss, detail::game_over>;
	state_t m_state;
	detail::bonus_t m_bonus;
	int m_frameID;
	int m_score;

public:
	bowling_t() : m_state(detail::game_start()), m_bonus(detail::bonus_t::zero), m_frameID(0), m_score(0){}
	~bowling_t() = default;
	auto get_state() const{
		return m_state;
	}

	auto get_score() const{
		return m_score;
	}

	auto get_bonus() const{
		return m_bonus;
	}

	auto get_frameID() const{
		return m_frameID;
	}

	void frame_for(const state_t& result){
		std::visit(
			detail::overloaded{
				[&](const detail::game_start& state){
					using namespace detail;
					if (std::holds_alternative<miss>(result)){
						auto temp = std::get<miss>(result);
						m_score += (temp.first_point + temp.second_point);
					}else if(std::holds_alternative<strike>(result)){
						m_score += ten_pins;
						m_bonus = std::get<strike>(result).bonus;
					}else{
						m_score += ten_pins;
						m_bonus = std::get<spare>(result).bonus;
					}
					++m_frameID;
					m_state = result;
				},
				[&](const detail::strike& state){
					std::cerr << "The game is end, your score is " << m_score << '\n';
				},
				[&](const detail::spare& state){
					std::cerr << "The game is end, your score is " << m_score << '\n';
				},
				[&](const detail::miss& state){
					std::cerr << "The game is end, your score is " << m_score << '\n';
				},
				[&](const detail::game_over& state){
					std::cerr << "The game is end, your score is " << m_score << '\n';
				}
			},
			m_state);
	}
};