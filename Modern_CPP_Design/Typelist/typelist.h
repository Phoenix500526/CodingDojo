#pragma once

template <bool flag, typename T, typename U>
struct Select
{
	using Result = T;
};
template <typename T, typename U>
struct Select<false, T, U>
{
	using Result = U;
};

template <typename... TList>
struct Typelist{};

using NullType = Typelist<>;

class EmptyType{};

template <int n>
struct Int2Type{
	enum { value = n };
};

#define TYPELIST(...) Typelist<__VA_ARGS__>


template <typename TList> struct Length;
template <typename... Tail>
struct Length<Typelist<Tail...>>
{
	enum { value = sizeof...(Tail) };
};


template <typename TList, std::size_t index> struct TypeAt;
template <typename Head, typename... Tail>
struct TypeAt<Typelist<Head, Tail...>, 0>
{
	using Result = Head;
};
template <typename Head, typename... Tail, std::size_t index>
struct TypeAt<Typelist<Head, Tail...>, index>
{
	static_assert(index < sizeof...(Tail) + 1, "TypeAt: Index out of bound!!\n");
	using Result = typename TypeAt<Typelist<Tail...>, index - 1>::Result;
};


template <typename TList, typename T> struct FindType;
template <typename T>
struct FindType<NullType, T>
{
	enum { value = -1 };
};
template <typename Head, typename... Tail>
struct FindType<Typelist<Head, Tail...>, Head>
{
	enum { value = 0 };
};
template <typename Head, typename... Tail, typename T>
struct FindType<Typelist<Head, Tail...>, T>
{
private:
	enum { temp = FindType<Typelist<Tail...>, T>::value};
public:
	enum { value = temp == -1 ? -1 : 1 + temp};
};

// 追加
template <typename T_1, typename T_2> struct Append;
template <typename T, typename... TList>
struct Append<Typelist<TList...>, T>
{
	using Result = Typelist<TList..., T>;
};
template <typename T, typename... TList>
struct Append<T, Typelist<TList...>>
{
	using Result = Typelist<T, TList...>;
};
template <typename... TList_1, typename... TList_2>
struct Append<Typelist<TList_1...>,Typelist<TList_2...> >
{
	using Result = Typelist<TList_1..., TList_2...>;
};

// 反转
template <typename TList> struct Reverse;
template <>
struct Reverse<NullType>
{
	using Result = NullType;
};
template <typename Head, typename... Tail>
struct Reverse<Typelist<Head, Tail...>>
{
	using Result = typename Append<typename Reverse<Typelist<Tail...>>::Result, Head>::Result;
};


template <typename TList, typename T> struct Erase;
template <typename T>
struct Erase<NullType, T>
{
	using Result = NullType;
};
template <typename Head, typename... Tail>
struct Erase<Typelist<Head, Tail...>, Head>
{
	using Result = Typelist<Tail...>;
};
template <typename Head, typename... Tail, typename T>
struct Erase<Typelist<Head, Tail...>, T>
{
	using Result = typename Append<Head, typename Erase<Typelist<Tail...>, T>::Result>::Result;
};


template <typename TList, typename T> struct EraseAll;
template <typename T>
struct EraseAll<NullType, T>
{
	using Result = NullType;
};
template <typename Head, typename... Tail>
struct EraseAll<Typelist<Head, Tail...>, Head>{
	using Result = typename EraseAll<Typelist<Tail...>, Head>::Result;
};
template <typename Head, typename... Tail, typename T>
struct EraseAll<Typelist<Head, Tail...>, T>
{
	using Result = typename Append<Head, typename EraseAll<Typelist<Tail...>, T>::Result>::Result;
};


template <typename TList, typename T, typename U> struct Replace;
template <typename T, typename U>
struct Replace<NullType, T, U>
{
	using Result = NullType;
};
template <typename Head, typename... Tail, typename U>
struct Replace<Typelist<Head, Tail...>, Head, U>
{
	using Result = Typelist<U, Tail...>;
};
template <typename Head, typename... Tail, typename T, typename U>
struct Replace<Typelist<Head, Tail...>, T, U>
{
	using Result = typename Append<Head, typename Replace<Typelist<Tail...>, T, U>::Result>::Result;
};

template <typename TList, typename T, typename U> struct ReplaceAll;
template <typename T, typename U>
struct ReplaceAll<NullType, T, U>
{
	using Result = NullType;
};
template <typename Head, typename... Tail, typename U>
struct ReplaceAll<Typelist<Head, Tail...>, Head, U>
{
	using Result = typename Append<U, typename ReplaceAll<Typelist<Tail...>, Head, U>::Result>::Result;
};
template <typename Head, typename... Tail, typename T, typename U>
struct ReplaceAll<Typelist<Head, Tail...>, T, U>
{
	using Result = typename Append<Head, typename ReplaceAll<Typelist<Tail...>, T, U>::Result>::Result;
};


template <typename TList> struct NoDuplicate;
template <>
struct NoDuplicate<NullType>{
	using Result = NullType;
};
template <typename Head, typename... Tail>
struct NoDuplicate<Typelist<Head, Tail...>>
{
private:
	using L1 = typename NoDuplicate<Typelist<Tail...>>::Result;
	using L2 = typename Erase<L1, Head>::Result;
public:
	using Result = typename Append<Head, L2>::Result;
};

template <typename TList, template<class> class Unit> class GenScatterHierarchy;
template <template<class> class Unit>
class GenScatterHierarchy<NullType, Unit>{};
template <typename AtomicType, template<class> class Unit>
class GenScatterHierarchy : public Unit<AtomicType>{};
template <typename Head, typename... Tail, template <class> class Unit>
class GenScatterHierarchy<Typelist<Head, Tail...>, Unit> 
	: public GenScatterHierarchy<Head, Unit>,
	  public GenScatterHierarchy<Typelist<Tail...>, Unit>{};

//template <typename Head, typename... Tail, std::size_t idx, template <class> class Unit>
template <typename Head, typename... Tail, template <typename> class Unit>
Unit<Head>& FieldHelper(GenScatterHierarchy<Typelist<Head, Tail...>, Unit>& obj, Int2Type<0>){
	GenScatterHierarchy<Head, Unit>& leftBase = obj;
	return leftBase;
}
template <typename Head, typename... Tail, std::size_t idx, template <typename> class Unit>
Unit<typename TypeAt<Typelist<Head, Tail...>, idx>::Result>& FieldHelper(GenScatterHierarchy<Typelist<Head, Tail...>, Unit>& obj, Int2Type<idx>){
	GenScatterHierarchy<Typelist<Tail...>, Unit>& rightBase = obj;
	return FieldHelper(rightBase, Int2Type<idx-1>());
}
template <int i, typename TList, template <typename> class Unit>
Unit<typename TypeAt<TList, i>::Result>& FieldHelper(GenScatterHierarchy<TList, Unit>& obj){
	return FieldHelper(obj, Int2Type<i>());
}


template <typename TList, 
		  template<class AtomicType, class Base> class Unit,
		  typename Root = EmptyType>
class GenLinearHierarchy;

template <
	template<class AtomicType, class Base> class Unit,
	typename Root>
class GenLinearHierarchy<NullType, Unit, Root> : public Root{};

template <
	typename Head,
	template<class AtomicType, class Base> class Unit,
	typename Root>
class GenLinearHierarchy<Typelist<Head>, Unit, Root>
	: public Unit<Head, Root>{};

template <
	typename Head,
	typename... Tail,
	template<class AtomicType, class Base> class Unit,
	typename Root>
class GenLinearHierarchy<Typelist<Head, Tail...>, Unit, Root>
	: public Unit<Head, GenLinearHierarchy<Typelist<Tail...>, Unit, Root>>{};