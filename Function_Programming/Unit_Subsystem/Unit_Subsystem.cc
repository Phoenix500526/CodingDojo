#include <iostream>
#include <ratio>
#include <boost/type_index.hpp>
using namespace std;
using boost::typeindex::type_id_with_cvr;

// 如果使用 string 保留单位名称会导致自定义字符串字面值出错，因为 string 有一个 non_trivial 的构造函数
template <class Rep, class Rat = std::ratio<1>>
struct Distance{
	using Ratio = Rat;
	Rep dis;
};

template <class Rep, class Rat = std::ratio<1>>
auto operator+(Distance<Rep, Rat> lhs, Distance<Rep, Rat> rhs){
	return Distance<Rep, Rat>{lhs.dis + rhs.dis, };
}

template <class Rep, class Rat = std::ratio<1>>
auto operator-(Distance<Rep, Rat> lhs, Distance<Rep, Rat> rhs){
	return Distance<Rep, Rat>{lhs.dis - rhs.dis};
}

template <class Rep>
using kilometers = Distance<Rep, std::kilo>;

template <class Rep>
using meters = Distance<Rep>;

template <class Rep>
using decimeters = Distance<Rep, std::deci>;

template <class Rep>
using centimeters = Distance<Rep, std::centi>;

template <class Rep>
using miles = Distance<Rep, std::ratio<1609344,1000>>;

template <class Rep>
using yards = Distance<Rep, std::ratio<9144, 10000>>;

template <class Rep>
using feet = Distance<Rep, std::ratio<3048, 10000>>;

template <class Rep>
using inches = Distance<Rep, std::ratio<354, 10000>>;


constexpr kilometers<long double> operator""_km(long double dist){
	return kilometers<long double>{dist};
}

constexpr meters<long double> operator""_m(long double dist){
	return meters<long double>{dist};
}

constexpr decimeters<long double> operator""_dm(long double dist){
	return decimeters<long double>{dist};
}

constexpr centimeters<long double> operator""_cm(long double dist){
	return centimeters<long double>{dist};
}

constexpr miles<long double> operator""_mi(long double dist){
	return miles<long double>{dist};
}

constexpr yards<long double> operator""_yd(long double dist){
	return yards<long double>{dist};
}

constexpr feet<long double> operator""_ft(long double dist){
	return feet<long double>{dist};
}

constexpr inches<long double> operator""_in(long double dist){
	return inches<long double>{dist};
}

template <class Rep>
ostream& operator<<(ostream& os, kilometers<Rep> len){
	return os << len.dis << "km";
}

template <class Rep>
ostream& operator<<(ostream& os, meters<Rep> len){
	return os << len.dis << "m";
}


template <class Rep>
ostream& operator<<(ostream& os, decimeters<Rep> len){
	return os << len.dis << "dm";
}


template <class Rep>
ostream& operator<<(ostream& os, centimeters<Rep> len){
	return os << len.dis << "cm";
}

template <class Rep>
ostream& operator<<(ostream& os, miles<Rep> len){
	return os << len.dis << "mi";
}

template <class Rep>
ostream& operator<<(ostream& os, yards<Rep> len){
	return os << len.dis << "yd";
}

template <class Rep>
ostream& operator<<(ostream& os, feet<Rep> len){
	return os << len.dis << "ft";
}

template <class Rep>
ostream& operator<<(ostream& os, inches<Rep> len){
	return os << len.dis << "in";
}


template <class To, class From>
constexpr To unit_cast(From from){
	using to_ratio = typename To::Ratio;
	using from_ratio = typename From::Ratio;
	using cast_ratio = std::ratio_divide<from_ratio, to_ratio>;
	return To{from.dis * cast_ratio::num / cast_ratio::den};
}


int main(int argc, char const *argv[])
{
	auto kms_1 = 10.24_km;
	auto kms_2 = 10.24_km;
	auto kms_3 = kms_1 + kms_2;
	auto mil_1 = 1.0_mi;
	auto mil_2 = 1.0_mi;
	auto mil_3 = mil_1 + mil_2;
	cout << type_id_with_cvr<decltype(kms_1)>().pretty_name() << endl;
	cout << type_id_with_cvr<decltype(mil_1)>().pretty_name() << endl;
	cout << kms_3 << endl;
	cout << mil_3 << endl;
	cout << unit_cast<kilometers<long double>>(mil_3) << endl;
	cout << unit_cast<meters<long double>>(mil_3) << endl;
	cout << unit_cast<decimeters<long double>>(mil_3) << endl;
	cout << unit_cast<centimeters<long double>>(mil_3) << endl;
	cout << unit_cast<miles<long double>>(kms_3) << endl;
	cout << unit_cast<yards<long double>>(kms_3) << endl;
	cout << unit_cast<feet<long double>>(kms_3) << endl;
	cout << unit_cast<inches<long double>>(kms_3) << endl;
	cout << unit_cast<yards<long double>>(1.0_mi) << endl;
	return 0;
}