#pragma once
#include <memory>
#include <assert.h>
#include "typelist.h"

template <typename R>
struct FunctorImplBase{
	using ResultType = R;
	using FunctorImplBaseType = FunctorImplBase<ResultType>;
	using Parm1 = EmptyType;
	using Parm2 = EmptyType;
	using Parm3 = EmptyType;
	using Parm4 = EmptyType;
	using Parm5 = EmptyType;
	using Parm6 = EmptyType;
	using Parm7 = EmptyType;
	using Parm8 = EmptyType;
	using Parm9 = EmptyType;
	using Parm10 = EmptyType;
	using Parm11 = EmptyType;
	using Parm12 = EmptyType;

	virtual ~FunctorImplBase(){}
	virtual FunctorImplBase* DoClone() const = 0;
	template <class U>
	static U* Clone(U* pObj){
		if(!pObj) return nullptr;
		U* pClone = static_cast<U*>(pObj->DoClone());
		assert(typeid(*pClone) == typeid(*pObj));
		return pClone;
	}
};

template <typename R, class TList> class FunctorImpl;



template <typename R>
class FunctorImpl<R, NullType> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	virtual ResultType operator()() = 0;
};


template <typename R, typename Arg1>
class FunctorImpl<R, Typelist<Arg1>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	virtual ResultType operator()(Parm1) = 0;
};

template <typename R, typename Arg1, typename Arg2>
class FunctorImpl<R, Typelist<Arg1, Arg2>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	virtual ResultType operator()(Parm1, Parm2) = 0;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3) = 0;

};

template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4) = 0;
};
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4, Arg5>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4, Arg5>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;
	using Parm5  = typename TypeTraits<ParamList,  4>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4, Parm5) = 0;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
		  typename Arg5, typename Arg6>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;
	using Parm5  = typename TypeTraits<ParamList,  4>::ParameterType;
	using Parm6  = typename TypeTraits<ParamList,  5>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4, 
								Parm5, Parm6) = 0;

};
template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
		  typename Arg5, typename Arg6, typename Arg7>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;
	using Parm5  = typename TypeTraits<ParamList,  4>::ParameterType;
	using Parm6  = typename TypeTraits<ParamList,  5>::ParameterType;
	using Parm7  = typename TypeTraits<ParamList,  6>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4, 
								Parm5, Parm6, Parm7) = 0;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
		  typename Arg5, typename Arg6, typename Arg7, typename Arg8>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;
	using Parm5  = typename TypeTraits<ParamList,  4>::ParameterType;
	using Parm6  = typename TypeTraits<ParamList,  5>::ParameterType;
	using Parm7  = typename TypeTraits<ParamList,  6>::ParameterType;
	using Parm8  = typename TypeTraits<ParamList,  7>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4, 
								Parm5, Parm6, Parm7, Parm8) = 0;
};

template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
		  typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,  Arg8, Arg9>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;
	using Parm5  = typename TypeTraits<ParamList,  4>::ParameterType;
	using Parm6  = typename TypeTraits<ParamList,  5>::ParameterType;
	using Parm7  = typename TypeTraits<ParamList,  6>::ParameterType;
	using Parm8  = typename TypeTraits<ParamList,  7>::ParameterType;
	using Parm9  = typename TypeTraits<ParamList,  8>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4, 
								Parm5, Parm6, Parm7, Parm8, Parm9) = 0;
};


template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
		  typename Arg5, typename Arg6, typename Arg7, typename Arg8, typename Arg9, 
		  typename Arg10>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,  Arg8, Arg9, Arg10>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;
	using Parm5  = typename TypeTraits<ParamList,  4>::ParameterType;
	using Parm6  = typename TypeTraits<ParamList,  5>::ParameterType;
	using Parm7  = typename TypeTraits<ParamList,  6>::ParameterType;
	using Parm8  = typename TypeTraits<ParamList,  7>::ParameterType;
	using Parm9  = typename TypeTraits<ParamList,  8>::ParameterType;
	using Parm10 = typename TypeTraits<ParamList,  9>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4, 
								Parm5, Parm6, Parm7, Parm8, 
								Parm9, Parm10) = 0;
};


template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
		  typename Arg5, typename Arg6, typename Arg7, typename Arg8,
		  typename Arg9, typename Arg10, typename Arg11>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7, Arg8, Arg9, Arg10, Arg11>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,  Arg8, Arg9, Arg10, Arg11>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;
	using Parm5  = typename TypeTraits<ParamList,  4>::ParameterType;
	using Parm6  = typename TypeTraits<ParamList,  5>::ParameterType;
	using Parm7  = typename TypeTraits<ParamList,  6>::ParameterType;
	using Parm8  = typename TypeTraits<ParamList,  7>::ParameterType;
	using Parm9  = typename TypeTraits<ParamList,  8>::ParameterType;
	using Parm10 = typename TypeTraits<ParamList,  9>::ParameterType;
	using Parm11 = typename TypeTraits<ParamList, 10>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4, 
								Parm5, Parm6, Parm7, Parm8, 
								Parm9, Parm10, Parm11) = 0;
};


template <typename R, typename Arg1, typename Arg2, typename Arg3, typename Arg4,
		  typename Arg5, typename Arg6, typename Arg7, typename Arg8,
		  typename Arg9, typename Arg10, typename Arg11, typename Arg12>
class FunctorImpl<R, Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,  Arg8, Arg9, Arg10, Arg11, Arg12>> : public FunctorImplBase<R>
{
public:
	using ResultType = R;
	using ParamList = Typelist<Arg1, Arg2, Arg3, Arg4, Arg5, Arg6, Arg7,  Arg8, Arg9, Arg10, Arg11, Arg12>;
	using Parm1  = typename TypeTraits<ParamList,  0>::ParameterType;
	using Parm2  = typename TypeTraits<ParamList,  1>::ParameterType;
	using Parm3  = typename TypeTraits<ParamList,  2>::ParameterType;
	using Parm4  = typename TypeTraits<ParamList,  3>::ParameterType;
	using Parm5  = typename TypeTraits<ParamList,  4>::ParameterType;
	using Parm6  = typename TypeTraits<ParamList,  5>::ParameterType;
	using Parm7  = typename TypeTraits<ParamList,  6>::ParameterType;
	using Parm8  = typename TypeTraits<ParamList,  7>::ParameterType;
	using Parm9  = typename TypeTraits<ParamList,  8>::ParameterType;
	using Parm10 = typename TypeTraits<ParamList,  9>::ParameterType;
	using Parm11 = typename TypeTraits<ParamList, 10>::ParameterType;
	using Parm12 = typename TypeTraits<ParamList, 11>::ParameterType;

	virtual ResultType operator()(Parm1, Parm2, Parm3, Parm4, 
								Parm5, Parm6, Parm7, Parm8, 
								Parm9, Parm10, Parm11, Parm12) = 0;
};

template <typename ParentFunctor, typename Func>
class FunctorHandler : public ParentFunctor::Impl
{
	using Base = typename ParentFunctor::Impl;
public:
	using ResultType = typename Base::ResultType;	
	using Parm1  = typename Base::Parm1;
	using Parm2  = typename Base::Parm2;
	using Parm3  = typename Base::Parm3;
	using Parm4  = typename Base::Parm4;
	using Parm5  = typename Base::Parm5;
	using Parm6  = typename Base::Parm6;
	using Parm7  = typename Base::Parm7;
	using Parm8  = typename Base::Parm8;
	using Parm9  = typename Base::Parm9;
	using Parm10 = typename Base::Parm10;
	using Parm11 = typename Base::Parm11;
	using Parm12 = typename Base::Parm12;

	FunctorHandler(const Func& func) : func_(func){}

	virtual FunctorHandler* DoClone() const{
		return new FunctorHandler(this->func_);
	}

	ResultType operator()() { return func_(); }

	ResultType operator()(Parm1 p1) { return func_(p1); }

	ResultType operator()(Parm1 p1, Parm2 p2) { 
		return func_(p1, p2); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) { 
		return func_(p1, p2, p3); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) { 
		return func_(p1, p2, p3, p4); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5) { 
		return func_(p1, p2, p3, p4, p5); 
	}


	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6) { 
		return func_(p1, p2, p3, p4, p5, p6); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7) { 
		return func_(p1, p2, p3, p4, p5, p6, p7); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8) { 
		return func_(p1, p2, p3, p4, p5, p6, p7, p8); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8, Parm9 p9) { 
		return func_(p1, p2, p3, p4, p5, p6, p7, p8, p9); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8, Parm9 p9, Parm10 p10) { 
		return func_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11) { 
		return func_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
				Parm12 p12) { 
		return func_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); 
	}
private:
	Func func_;
};

template <typename ParentFunctor, typename PointerToObj, typename PointerToMemFn>
class MemFunHandler : public ParentFunctor::Impl
{
	using Base = typename ParentFunctor::Impl;
public:
	using ResultType = typename Base::ResultType;
	using Parm1  = typename Base::Parm1;
	using Parm2  = typename Base::Parm2;
	using Parm3  = typename Base::Parm3;
	using Parm4  = typename Base::Parm4;
	using Parm5  = typename Base::Parm5;
	using Parm6  = typename Base::Parm6;
	using Parm7  = typename Base::Parm7;
	using Parm8  = typename Base::Parm8;
	using Parm9  = typename Base::Parm9;
	using Parm10 = typename Base::Parm10;
	using Parm11 = typename Base::Parm11;
	using Parm12 = typename Base::Parm12;

	MemFunHandler(const PointerToObj& pObj, PointerToMemFn pMemFun)
		: pObj_(pObj), pMemFun_(pMemFun){}

	virtual MemFunHandler* DoClone() const{
		return new MemFunHandler(this->pObj_, this->pMemFun_);
	}

	ResultType operator()(){
		return ((*pObj_).*pMemFun_)();
	}

	ResultType operator()(Parm1 p1){
		return ((*pObj_).*pMemFun_)(p1);
	}

	ResultType operator()(Parm1 p1, Parm2 p2) { 
		return ((*pObj_).*pMemFun_)(p1, p2); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4, p5); 
	}


	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4, p5, p6); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4, p5, p6, p7); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4, p5, p6, p7, p8); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8, Parm9 p9) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4, p5, p6, p7, p8, p9); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8, Parm9 p9, Parm10 p10) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11); 
	}

	ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
				Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
				Parm8 p8, Parm9 p9, Parm10 p10, Parm11 p11,
				Parm12 p12) { 
		return ((*pObj_).*pMemFun_)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12); 
	}
private:
	PointerToObj pObj_;
	PointerToMemFn pMemFun_;
};

class bad_function_call : public std::runtime_error
{
public:
	bad_function_call() : std::runtime_error("bad_function_call in Functor"){}	
};

template <typename R = void, typename TList = NullType>
class Functor{
public:
	using Impl = FunctorImpl<R, TList>;
	using ResultType = R;
	using ParmList = TList;
	using Parm1  = typename Impl::Parm1;
	using Parm2  = typename Impl::Parm2;
	using Parm3  = typename Impl::Parm3;
	using Parm4  = typename Impl::Parm4;
	using Parm5  = typename Impl::Parm5;
	using Parm6  = typename Impl::Parm6;
	using Parm7  = typename Impl::Parm7;
	using Parm8  = typename Impl::Parm8;
	using Parm9  = typename Impl::Parm9;
	using Parm10 = typename Impl::Parm10;
	using Parm11 = typename Impl::Parm11;
	using Parm12 = typename Impl::Parm12;

	Functor() : spImpl_(nullptr){}
	Functor(const Functor& rhs) : spImpl_(Impl::Clone(rhs.spImpl_.get())){}
	Functor(std::unique_ptr<Impl>&& spImpl) : spImpl_(spImpl){}

	template <typename Func>
	Functor(Func func) : spImpl_(new FunctorHandler<Functor, Func>(func)){}

	template <typename PtrObj, typename MemFunc>
	Functor(const PtrObj& pObj, MemFunc func) : spImpl_(new MemFunHandler<Functor, PtrObj, MemFunc>(pObj, func)){} 

	Functor& operator=(const Functor& rhs) {
        Functor copy(rhs);
        Impl* p = spImpl_.release();
        spImpl_.reset(copy.spImpl_.release());
        copy.spImpl_.reset(p);
        return *this;
    }

    bool empty() const {
        return spImpl_.get() == nullptr;
    }

    void clear() {
        spImpl_.reset(nullptr);
    }

    ResultType operator()() const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)();
    }

    ResultType operator()(Parm1 p1) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1);
    }


	ResultType operator()(Parm1 p1, Parm2 p2) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, Parm4 p4, Parm5 p5) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4, p5);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
    			Parm4 p4, Parm5 p5, Parm6 p6) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4, p5, p6);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
    			Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
    			Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
    			Parm8 p8) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, p8);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
    			Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
    			Parm8 p8, Parm9 p9) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, 
    					  p8, p9);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
    			Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
    			Parm8 p8, Parm9 p9, Parm10 p10) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, 
    					  p8, p9, p10);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
    			Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
    			Parm8 p8, Parm9 p9, Parm10 p10, 
    			Parm11 p11) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, 
    					  p8, p9, p10, p11);
    }

    ResultType operator()(Parm1 p1, Parm2 p2, Parm3 p3, 
    			Parm4 p4, Parm5 p5, Parm6 p6, Parm7 p7,
    			Parm8 p8, Parm9 p9, Parm10 p10, 
    			Parm11 p11, Parm12 p12) const {
    	if(empty()) throw bad_function_call();
    	return (*spImpl_)(p1, p2, p3, p4, p5, p6, p7, 
    					  p8, p9, p10, p11, p12);
    }
private:
	std::unique_ptr<Impl> spImpl_;
};

