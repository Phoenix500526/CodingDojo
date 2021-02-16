#pragma once

#include <functional>
#include <vector>
#include "TypeInfo.h"
#include "typelist.h"
#include "AssocVector.h"
#include "Functor.h"

template 
<
    class Executor,
    class BaseLhs,
    class TypeLhs,
    bool symmetric = true, 
    class BaseRhs = BaseLhs,
    class TypeRhs = TypeLhs,
    typename ResultType = void
>
class StaticDispatcher
{
    using TLhs = typename DerivedToFront<TypeLhs>::Result; 
    using TRhs = typename DerivedToFront<TypeRhs>::Result;
private:
    template <typename SomeLhs>
    static ResultType DispatchRhs(SomeLhs& lhs, BaseRhs& rhs, Executor exec, NullType){
        return exec.OnError(lhs, rhs);
    }
    template <typename Head, typename... Tail, typename SomeLhs>
    static ResultType DispatchRhs(SomeLhs& lhs, BaseRhs& rhs, Executor exec, Typelist<Head, Tail...>){
        if(Head* p = dynamic_cast<Head*>(&rhs)){
            Int2Type<symmetric &&
                static_cast<int>(FindType<TRhs, Head>::value) < static_cast<int>(FindType<TLhs, SomeLhs>::value)
            > tag;
            return InvocationTraits<SomeLhs, Head>::DoDispatch(lhs, *p, exec, tag);
        }
        return DispatchRhs(lhs, rhs, exec, Typelist<Tail...>());
    }
    static ResultType DispatchLhs(BaseLhs& lhs, BaseRhs& rhs, Executor exec, NullType){
        return exec.OnError(lhs, rhs);
    }
    template <typename Head, typename... Tail>
    static ResultType DispatchLhs(BaseLhs& lhs, BaseRhs& rhs, Executor exec, Typelist<Head, Tail...>){
        if(Head* p = dynamic_cast<Head*>(&lhs)){
            return DispatchRhs(*p, rhs, exec, TRhs());
        }
        return DispatchLhs(lhs, rhs, exec, Typelist<Tail...>());
    }
public:
    static ResultType Go(BaseLhs& lhs, BaseRhs& rhs, Executor exec){
        return DispatchLhs(lhs, rhs, exec, TLhs());
    }
    template <typename SomeLhs, typename SomeRhs>
    struct InvocationTraits
    {
        static ResultType DoDispatch(SomeLhs& lhs, SomeRhs& rhs, Executor& exec, Int2Type<false>){
            return exec.Fire(lhs, rhs);
        }
        static ResultType DoDispatch(SomeLhs& lhs, SomeRhs& rhs, Executor& exec, Int2Type<true>){
            return exec.Fire(rhs, lhs);
        }
    };
};

template <
    class BaseLhs,
    class BaseRhs = BaseLhs,
    typename ResultType = void,
    typename CallbackType = std::function<ResultType(BaseLhs&, BaseRhs&)>
>
class BasicDispatcher
{
    using KeyType = std::pair<TypeInfo, TypeInfo>;
    using ValueType = CallbackType;
    using MapType = AssocVector<KeyType, ValueType>;
    MapType callbackMap_;

    void DoAdd(const TypeInfo& lhs,const TypeInfo& rhs, CallbackType func){
        callbackMap_[KeyType(lhs, rhs)] = func;
    }
    bool DoRemove(const TypeInfo& lhs,const TypeInfo& rhs){
        return callbackMap_.erase(KeyType(lhs, rhs));
    }
public:
    template <class SomeLhs, class SomeRhs>
    void Add(CallbackType fun){
        DoAdd(typeid(SomeLhs), typeid(SomeRhs), fun);
    }
    template <class SomeLhs, class SomeRhs>
    bool Remove(){
        return DoRemove(typeid(SomeLhs), typeid(SomeRhs));
    }
    ResultType Go(BaseLhs& lhs, BaseRhs& rhs){
        typename MapType::key_type key(typeid(lhs), typeid(rhs));
        typename MapType::iterator iter = callbackMap_.find(key);
        if(iter == callbackMap_.end()){
            throw std::runtime_error("Function Not Found");
        }
        return (iter->second)(lhs, rhs);
    }
};


template <
    class BaseLhs,
    class BaseRhs = BaseLhs,
    typename ResultType = void,
    typename CallbackType = std::function<ResultType(BaseLhs&, BaseRhs&)>
>
class BasicFastDispatcher
{
private:
    using Row = std::vector<CallbackType>;
    using Matrix = std::vector<Row>;
    Matrix callback_;
    int columns_;
public:
    template <class SomeLhs, class SomeRhs>
    void Add(CallbackType pFunc){
        int& idxLhs = SomeLhs::GetClassIndexStatic();
        if (idxLhs < 0){
            callback_.push_back(Row());
            idxLhs = callback_.size() - 1;
        }else if(callback_.size() <= idxLhs){
            callback_.resize(idxLhs + 1);
        }
        Row& thisRow = callback_[idxLhs];
        int& idxRhs = SomeRhs::GetClassIndexStatic();
        if(idxRhs < 0){
            thisRow.resize(++columns_);
            idxRhs = thisRow.size() - 1;
        }else if(thisRow.size() <= idxRhs){
            thisRow.resize(idxRhs + 1);
        }
        thisRow[idxRhs] = pFunc;
    }

    template <class SomeLhs, class SomeRhs>
    bool Remove(){
        int& idxLhs = SomeLhs::GetClassIndexStatic();
        int& idxRhs = SomeRhs::GetClassIndexStatic();
        callback_[idxLhs][idxRhs] = nullptr;
        return nullptr == callback_[idxLhs][idxRhs];
    }

    BasicFastDispatcher() : columns_(0){}
    ResultType Go(BaseLhs& lhs, BaseRhs& rhs){
        int idxLhs = lhs.GetClassIndex();
        int idxRhs = rhs.GetClassIndex();
        if(idxLhs < 0 || idxLhs >= callback_.size() || idxRhs > 0 || idxRhs >= callback_[idxLhs].size()
                || callback_[idxLhs][idxRhs] == nullptr){
            throw std::runtime_error("Function Not Found");
        }
        return callback_[idxLhs][idxRhs](lhs, rhs);
    }
    
};

#define IMPLEMENT_INDEXABLE_CLASS(SomeClass)        \
static int& GetClassIndexStatic(){                  \
    static int index = -1;                          \
    return index;                                   \
}                                                   \
virtual int& GetClassIndex(){                       \
    assert( typeid(*this) == typeid(SomeClass));    \
    return GetClassIndexStatic();                   \
}


// StaticCast 和 DynamicCast 作为 Policy 必须是对象类型
template <class To, class From>
struct StaticCast
{
    static To& Cast(From& from){
        return static_cast<To&>(from);
    }
};

template <class To, class From>
struct DynamicCast
{
    static To& Cast(From& from){
        return dynamic_cast<To&>(from);
    }
};

// 最后一个函数指针代表了非类型参数
template <class BaseLhs, class BaseRhs,
          class SomeLhs, class SomeRhs,
          typename ResultType,
          class CastLhs, class CastRhs,
          ResultType (*Callback)(SomeLhs&, SomeRhs&)>
struct FnDispatcherHelper
{
    static ResultType Trampoline(BaseLhs& lhs, BaseRhs& rhs)
    {
        return Callback(CastLhs::Cast(lhs), CastRhs::Cast(rhs));
    }
    static ResultType TrampolineR(BaseRhs& rhs, BaseLhs& lhs)
    {
        return Trampoline(lhs, rhs);
    }
};

template <class BaseLhs, class BaseRhs = BaseLhs,
          typename ResultType = void,
          template <class, class> class CastingPolicy = DynamicCast,
          template <class, class, class, class> class DispatcherBackend = BasicFastDispatcher>
class FnDispatcher
{
    DispatcherBackend<BaseLhs, BaseRhs, ResultType, ResultType (*)(BaseLhs&, BaseRhs&)> backEnd_;

public:
    template <class SomeLhs, class SomeRhs>
    void Add(ResultType (*pFunc)(BaseLhs&, BaseRhs&)){
        return backEnd_.template Add<SomeLhs, SomeRhs>(pFunc);
    }

    template <class SomeLhs, class SomeRhs,
        ResultType (*callback)(SomeLhs&, SomeRhs&)>
    void Add(){
        using Local = FnDispatcherHelper<
            BaseLhs, BaseRhs, SomeLhs, SomeRhs,
            ResultType, CastingPolicy<SomeLhs, BaseLhs>,
            CastingPolicy<SomeRhs, BaseRhs>, callback>;
        Add<SomeLhs, SomeRhs>(&Local::Trampoline);
    }
    template <class SomeLhs, class SomeRhs>
    void Remove(){
        backEnd_.template Remove<SomeLhs, SomeRhs>();
    }
    ResultType Go(BaseLhs& lhs, BaseRhs& rhs){
        return backEnd_.Go(lhs, rhs);
    }
    
};

template <class BaseLhs, class BaseRhs,
          class SomeLhs, class SomeRhs,
          typename ResultType,
          class CastLhs, class CastRhs,
          class Fun, bool SwapArgs>
class FunctorDispatcherHelper
{
private:
    Fun fun_;
    ResultType Fire(BaseLhs& lhs, BaseRhs& rhs, Int2Type<false>){
        return fun_(CastLhs::Cast(lhs), CastRhs::Cast(rhs));
    }
    ResultType Fire(BaseLhs& lhs, BaseRhs& rhs, Int2Type<true>){
        return fun_(CastLhs::Cast(rhs), CastRhs::Cast(lhs));
    }
public:
    FunctorDispatcherHelper(const Fun& fun) : fun_(fun){}
    ResultType operator()(BaseLhs& lhs, BaseRhs& rhs){
        return Fire(lhs, rhs, Int2Type<SwapArgs>());
    }
};

template <class BaseLhs, class BaseRhs = BaseLhs,
          typename ResultType = void,
          template <class, class> class CastingPolicy = DynamicCast,
          template <class, class, class, class> class DispatcherBackend = BasicDispatcher>
class FunctorDispatcher
{
    using ArgsList = TYPELIST(BaseLhs&, BaseRhs&);
    using FunctorType = Functor<ResultType, ArgsList>;
    DispatcherBackend<BaseLhs, BaseRhs, ResultType, FunctorType> backEnd_;
public:

    template <class SomeLhs, class SomeRhs, class Fun>
    void Add(const Fun& fun){
        using Adapter = FunctorDispatcherHelper<
            BaseLhs, BaseRhs,
            SomeLhs, SomeRhs,
            ResultType,
            CastingPolicy<SomeLhs, BaseLhs>,
            CastingPolicy<SomeRhs, BaseRhs>,
            Fun, false
        >;
        backEnd_.template Add<SomeLhs, SomeRhs>(static_cast<FunctorType>(Adapter(fun)));
    }

    template <class SomeLhs, class SomeRhs, bool symmetric, class Fun>
    void Add(const Fun& fun){
        Add<SomeLhs, SomeRhs>(fun);
        if(symmetric){
            using AdapterR = FunctorDispatcherHelper<
                BaseLhs, BaseLhs,
                SomeLhs, SomeRhs,
                ResultType,
                CastingPolicy<SomeLhs, BaseLhs>,
                CastingPolicy<SomeRhs, BaseLhs>,
                Fun, true
            >;
            backEnd_.template Add<SomeRhs, SomeLhs>(static_cast<FunctorType>(AdapterR(fun)));
        }
    }
    
    template <class SomeLhs, class SomeRhs>
    void Remove(){
        backEnd_.template Remove<SomeLhs, SomeRhs>();
    }

    ResultType Go(BaseLhs& lhs, BaseRhs& rhs){
        return backEnd_.Go(lhs, rhs);
    }
};