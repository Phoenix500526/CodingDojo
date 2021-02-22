#include <iostream>
#include <functional>
#include <tuple>
using namespace std;

template <class Function, class... CapturedArgs>
class curried{
private:
	using CapturedArgsTuple = std::tuple<std::decay_t<CapturedArgs>...>;
	template <class... Args>
	static auto capture_by_value(Args&&... args){
		return std::tuple<std::decay_t<Args>...>(std::forward<Args>(args)...);
	}
public:
	curried(Function function, CapturedArgs&&... args)
		: m_function(function), m_capture(capture_by_value(std::move(args)...)){}

	curried(Function function, std::tuple<CapturedArgs...> args)
		: m_function(function), m_capture(std::move(args)){}

	template <class... NewArgs>
	auto operator()(NewArgs&&... args){
		//注意此处不能使用 make_tuple 作为 new_args 的构造方法，make_tuple 是基于值拷贝的，具体可见：
		// https://stackoverflow.com/questions/66300432/how-to-pass-a-rvalue-reference-parameter-to-a-template-operator-function-in-c
		//auto new_args = std::make_tuple(std::forward<NewArgs>(args)...);
		auto new_args = std::tuple<NewArgs...>(std::forward<NewArgs>(args)...);
        auto all_args = std::tuple_cat(m_capture, std::move(new_args));
		if constexpr(std::is_invocable_v<Function, CapturedArgs..., NewArgs...>){
			return std::apply(m_function, std::move(all_args));
		}else{
			return curried<Function, CapturedArgs..., NewArgs...>(m_function, all_args);
		}
	}
private:
	Function m_function;
	std::tuple<CapturedArgs...> m_capture;
};

void func_1(string& str1, string str2, string str3){
	cout << "func_2 : str1 = " << str1 
		 << ", str2 = " << str2
		 << ", str3 = " << str3 << endl;
}

void func_2(string& str1, string str2, string&& str3){
	cout << "func_3 : str1 = " << str1 
		 << ", str2 = " << str2 
		 << ", str3 = " << str3 << endl;
}

int main(int argc, char const *argv[])
{
	string str1 = "Hello ", str2 = "World", str3 = "!", str4 = "abc";
	auto test_1 = curried(func_1);	// For the first question
	cout << "result : ";
	test_1(std::ref(str1))(str2)(str3);	// Work perfectly

	auto test_2 = curried(func_2);
	cout << "result : ";
	test_2(std::ref(str1))(str2)(std::move(str3));	
	return 0;
}