#include <iostream>
#include <boost/type_index.hpp>
#include "typelist.h"
using namespace std;
using boost::typeindex::type_id_with_cvr;

template <typename TList> struct Typelist_trait;
template <typename H, typename... T>
struct Typelist_trait<Typelist<H, T...>>
{
	using Head = H;
	using Tail = Typelist<T...>;
};

template <typename TList>
void TList_Printer(){
	using TypeList = Typelist_trait<TList>;
	cout << type_id_with_cvr<typename TypeList::Head>().pretty_name() << ',';
	TList_Printer<typename TypeList::Tail>();
}
template<>
void TList_Printer<NullType>(){
	cout << "NullType\n";
}


struct Widget{};
struct Button:Widget{};
struct GraphicButton : Button{};
struct Scrollbar : Widget{};
struct Window{};

template <typename T>
struct Holder{
	T value_;
};

template <class T, class Base>
class EventHandler : public Base{
public:
	void OnEvent(T& obj){
		cout << type_id_with_cvr<T>().pretty_name() << '\n';
	}
};

int main(){
	using tl_0 = TYPELIST();
	using tl_1 = TYPELIST(int);
	using tl_2 = TYPELIST(char, int);
	using tl_3 = TYPELIST(char, int, short);
	TList_Printer<tl_0>();
	cout << "Length of tl_0 is " << Length<tl_0>::value << '\n';
	TList_Printer<tl_1>();
	cout << "Length of tl_1 is " << Length<tl_1>::value << '\n';
	TList_Printer<tl_2>();
	cout << "Length of tl_2 is " << Length<tl_2>::value << '\n';
	TList_Printer<tl_3>();
	cout << "Length of tl_3 is " << Length<tl_3>::value << '\n';
	cout << type_id_with_cvr<typename TypeAt<tl_3, 0>::Result>().pretty_name() << ','
		 << type_id_with_cvr<typename TypeAt<tl_3, 1>::Result>().pretty_name() << ','
		 << type_id_with_cvr<typename TypeAt<tl_3, 2>::Result>().pretty_name() << '\n';
	cout << FindType<tl_3, char>::value << ',' 
		 << FindType<tl_3, int>::value << ',' 
		 << FindType<tl_3, short>::value << ','
		 << FindType<tl_3, float>::value << '\n';
	cout << "\nAppend test : \n";
	cout << type_id_with_cvr<typename Append<tl_0, tl_0>::Result>().pretty_name() << '\n'
		 << type_id_with_cvr<typename Append<tl_0, double>::Result>().pretty_name() << '\n'
		 << type_id_with_cvr<typename Append<double, tl_0>::Result>().pretty_name() << '\n'
		 << type_id_with_cvr<typename Append<tl_0, tl_3>::Result>().pretty_name() << '\n'
		 << type_id_with_cvr<typename Append<tl_2, tl_3>::Result>().pretty_name() << '\n'
		 << type_id_with_cvr<typename Append<double, tl_2>::Result>().pretty_name() << '\n'
		 << type_id_with_cvr<typename Append<tl_2, double>::Result>().pretty_name() << '\n';

	using tl_4 = typename Append<tl_2, tl_3>::Result;
	cout << "\nReverse test : \n";
	TList_Printer<typename Reverse<tl_0>::Result>();
	TList_Printer<typename Reverse<tl_1>::Result>();
	TList_Printer<tl_2>();
	TList_Printer<typename Reverse<tl_2>::Result>();
	TList_Printer<typename Reverse<tl_3>::Result>();

	cout << "\nErase test : \n";
	TList_Printer<typename Erase<tl_3, double>::Result>();
	TList_Printer<typename Erase<tl_3, int>::Result>();

	cout << "\nEraseAll test : \n";
	TList_Printer<typename EraseAll<typename Append<tl_2, tl_3>::Result, int>::Result>();
	TList_Printer<typename EraseAll<tl_3, int>::Result>();
	TList_Printer<typename EraseAll<tl_3, double>::Result>();

	cout << "\nReplace test : \n";
	TList_Printer<typename Replace<tl_0, int, double>::Result>();
	TList_Printer<typename Replace<tl_3, int, double>::Result>();
	TList_Printer<typename Replace<tl_3, double, int>::Result>();

	cout << "\nReplaceAll test : \n";
	TList_Printer<typename ReplaceAll<tl_0, int, double>::Result>();
	TList_Printer<typename ReplaceAll<tl_3, int, double>::Result>();
	TList_Printer<typename ReplaceAll<tl_4, char , float>::Result>();

	cout << "\nNoDuplicate test : \n";
	TList_Printer<tl_4>();
	TList_Printer<typename NoDuplicate<tl_4>::Result>();
	using ScatterHierarchy = GenScatterHierarchy<TYPELIST(int, float, double), Holder>;
	ScatterHierarchy test{1024, 3.14, 2.71};
	cout << sizeof(test) << " : " << static_cast<Holder<int>&>(test).value_ 
		 << " : " << static_cast<Holder<float>&>(test).value_ 
		 << " : " << static_cast<Holder<double>&>(test).value_ << '\n';

	cout << type_id_with_cvr<decltype(FieldHelper<0>(test).value_)>().pretty_name() << ','
		 << type_id_with_cvr<decltype(FieldHelper<1>(test).value_)>().pretty_name() << ','
		 << type_id_with_cvr<decltype(FieldHelper<2>(test).value_)>().pretty_name() << '\n';

	using LinearHierarchy = GenLinearHierarchy<TYPELIST(Window, Button, Scrollbar), EventHandler>;
	LinearHierarchy linear;
	return 0;
}