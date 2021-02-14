#include <iostream>
#include "Functor.h"

using namespace std;

void TestFunction(int i, double d){
	cout << "TestFunction(" << i << ", " << d << ") called.\n";
}

struct Parrot{
	void Eat(){
		cout << "Parrot::Eat()\n";
	}
	void Speak(){
		cout << "Parrot::Speak()\n";
	}
};

int main(){
	Functor<void, TYPELIST(double, double)> cmd(TestFunction);
	cmd(1024, 3.14);
	Parrot parrot;
	Functor<> cmd_1(&parrot, &Parrot::Eat), cmd_2(&parrot, &Parrot::Speak);
	cmd_1();
	cmd_2();
	return 0;
}