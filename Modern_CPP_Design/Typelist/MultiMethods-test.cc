#include <iostream>
#include "MultiMethods.h"
using namespace std;


struct Shape{
	virtual ~Shape(){}
	IMPLEMENT_INDEXABLE_CLASS(Shape)
};
struct Retangle : public Shape{
	virtual ~Retangle(){}
	IMPLEMENT_INDEXABLE_CLASS(Retangle)
};
struct Ellipse : public Shape{
	virtual ~Ellipse(){}
	IMPLEMENT_INDEXABLE_CLASS(Ellipse)
};

struct Poly : public Shape{
	virtual ~Poly(){}
	IMPLEMENT_INDEXABLE_CLASS(Poly)
};

struct RoundRetangle : public Retangle{
	virtual ~RoundRetangle(){}
	IMPLEMENT_INDEXABLE_CLASS(RoundRetangle)
};

class HatchingExecutor
{
public:
	static void Fire(RoundRetangle&, RoundRetangle&){
		cout << "RoundRetangle, RoundRetangle\n";
	}

	void Fire(RoundRetangle&, Retangle&){
		cout << "RoundRetangle, Retangle\n";
	}

	void Fire(RoundRetangle&, Ellipse&){
		cout << "RoundRetangle, Ellipse\n";
	}

	void Fire(Retangle&, Retangle&){
		cout << "Retangle, Retangle\n";
	}
	void Fire(Retangle&, Ellipse&){
		cout << "Retangle, Ellipse\n";
	}

	static void Fire(Ellipse&, Ellipse&){
		cout << "Ellipse, Ellipse\n";
	}

	void OnError(Shape&, Shape&){
		throw std::runtime_error("Error!!");
	}

};

void HatchRetanglePoly(Shape& lhs, Shape& rhs){
	cout << "HatchRetanglePoly:" << typeid(lhs).name() << " , " << typeid(rhs).name() << '\n';
}

void HatchEllipsePoly(Ellipse& lhs, Poly& rhs){
	cout << "HatchEllipsePoly:" << typeid(lhs).name() << " , " << typeid(rhs).name() << '\n';
	return;
}

struct HatchRetangleEllipse_1
{
	void operator()(Retangle& lhs, Ellipse& rhs){
		cout << "HatchRetangleEllipse_1:" << typeid(lhs).name() << " , " << typeid(rhs).name() << '\n';
	}
};

struct HatchRoundRantanglePoly
{
	void operator()(RoundRetangle& lhs, Poly& rhs){
		cout << "HatchRoundRantanglePoly:" << typeid(lhs).name() << " , " << typeid(rhs).name() << '\n';
	}
};

int main(){
	using SDispatcher = StaticDispatcher<HatchingExecutor, Shape, TYPELIST(RoundRetangle, Retangle, Ellipse)>;
	Retangle ret;
	Ellipse ell;
	Poly pol;
	RoundRetangle rou;
	HatchingExecutor exec;
	SDispatcher s_dispatcher;
	s_dispatcher.Go(ret, ell, exec);
	s_dispatcher.Go(rou, ell, exec);
	s_dispatcher.Go(rou, rou, exec);
	using BDispatcher = BasicDispatcher<Shape>;
	BDispatcher b_dispatcher;
	auto HatchRetangleEllipse = [](Shape& lhs, Shape& rhs){
		cout << "HatchRetangleEllipse:" << typeid(lhs).name() << " , " << typeid(rhs).name() << '\n';
	};
	b_dispatcher.Add<Retangle, Poly>(HatchRetanglePoly);
	b_dispatcher.Add<Retangle, Ellipse>(HatchRetangleEllipse);
	b_dispatcher.Go(ret, pol);
	b_dispatcher.Go(ret, ell);

	FnDispatcher<Shape> fn_dispatcher;
	fn_dispatcher.Add<Ellipse, Poly,HatchEllipsePoly>();
	fn_dispatcher.Go(ell, pol);

	FunctorDispatcher<Shape> fctor_dispatcher;
	fctor_dispatcher.Add<Retangle, Ellipse, true>(HatchRetangleEllipse_1());
	fctor_dispatcher.Go(ret, ell);
	fctor_dispatcher.Go(ell, ret);
	fctor_dispatcher.Add<RoundRetangle, Poly>(HatchRoundRantanglePoly());
	fctor_dispatcher.Go(rou, pol);

	using BFDispatcher = BasicFastDispatcher<Shape>;
	BFDispatcher bf_dispatcher;
	bf_dispatcher.Add<Retangle, Poly>(HatchRetanglePoly);
	bf_dispatcher.Go(ret, pol);
	return 0;

}