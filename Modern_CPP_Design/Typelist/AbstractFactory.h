#pragma once
#include <memory>
#include "typelist.h"

/* 对于抽象工厂而言，只关心接口，不关心实现
 */
template <typename T>
class AFUnit
{
public:
	virtual T* DoCreate(Type2Type<T>) = 0;
	virtual ~AFUnit(){}
};

template <typename TList, template <class> class Unit = AFUnit>
class AbstractFactory : public GenScatterHierarchy<TList, Unit>
{
public:
	using ProductList = TList;
	template <typename T, typename... Args>
	std::shared_ptr<T> Create(Args&&... args){
		auto Product = std::shared_ptr<T>(static_cast<Unit<T>&>(*this).DoCreate(Type2Type<T>()));
		Product->Init(std::forward<Args>(args)...);
		return Product;
	}
};


/* 具体工厂需要考虑接口定义(由 AbstractFactory Policy 表示)，实现方式(由 OpNewFactoryUnit 表示)，
 */
template <typename ConcreteProduct, typename Base>
class OpNewFactoryUnit : public Base
{
private:
	using BaseProductList = typename Base::ProductList;
protected:
	using ProductList = typename TList_Trait<BaseProductList>::Tail;
public:
	using AbstractProduct = typename TList_Trait<BaseProductList>::Head;
	ConcreteProduct* DoCreate(Type2Type<AbstractProduct>){
		return new ConcreteProduct;
	}
};

template <typename ConcreteProduct, typename Base>
class PrototypeFactoryUnit : public Base
{
private:
	using BaseProductList = typename Base::ProductList;
protected:
	using ProductList = typename TList_Trait<BaseProductList>::Tail;
public:
	using AbstractProduct = typename TList_Trait<BaseProductList>::Head;

	PrototypeFactoryUnit(AbstractProduct* p = nullptr) : pPrototype_(p){}

	template <class CP, class Base_1>
	friend void DoGetPrototype(const PrototypeFactoryUnit<CP, Base_1>& self, 
				typename TList_Trait<typename Base_1::ProductList>::Head*& pPrototype);

	template <class CP, class Base_1>
	friend void DoSetPrototype(PrototypeFactoryUnit<CP, Base_1>& self, 
				typename TList_Trait<typename Base_1::ProductList>::Head* pPrototype);

	template <typename U>
	void GetPrototype(U*& pObj){
		DoGetPrototype(*this, pObj);
	}
	template <typename U>
	void SetPrototype(U* pObj){
		DoSetPrototype(*this, pObj);
	}
	AbstractProduct* DoCreate(Type2Type<AbstractProduct>){
		assert(pPrototype_);
		return pPrototype_->Clone();
	}
private:
	AbstractProduct* pPrototype_;
};

template <class ConcreteProduct, class Base>
inline void DoGetPrototype(const PrototypeFactoryUnit<ConcreteProduct, Base>& self,
					typename TList_Trait<typename Base::ProductList>::Head*& pPrototype){
	pPrototype = self.pPrototype_;
}

template <class ConcreteProduct, class Base>
inline void DoSetPrototype(PrototypeFactoryUnit<ConcreteProduct, Base>& self,
					typename TList_Trait<typename Base::PrototypeFactoryUnit>::Head* pPrototype){
	self.pPrototype_ = pPrototype;
}

template <
	class AbstractFactory,
	template <class, class> class Creator = OpNewFactoryUnit,
	typename TList = typename AbstractFactory::ProductList>
class ConcreteFactory : public GenLinearHierarchy<typename Reverse<TList>::Result, Creator, AbstractFactory>
{
public:
	using ProductList = typename AbstractFactory::ProductList;
	using ConcreteProductList = TList;
};