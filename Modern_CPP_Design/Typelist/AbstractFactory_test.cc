#include <iostream>
#include "AbstractFactory.h"
using namespace std;

struct Soldier
{
	int HP;
	int MP;
	int Attack;
	int Defence;
	void Init(int hp, int mp, int atck, int def){
		HP = hp;
		MP = mp;
		Attack = atck;
		Defence = def;
	}
	virtual void func(){
		cout << "I'm Soldier\n";
	}
	virtual ~Soldier(){}
};

struct Monster
{
	int HP;
	int Attack;
	int Defence;
	virtual void func(){
		cout << "I'm Monster \n";
	}
	void Init(int hp,int atck, int def){
		HP = hp;
		Attack = atck;
		Defence = def;
	}
	virtual ~Monster(){}
};

struct SuperMonster
{
	int HP;
	int Attack;
	int Defence;
	int AddationalDamage;
	void Init(int hp,int atck, int def, int AdaDmg){
		HP = hp;
		Attack = atck;
		Defence = def;
		AddationalDamage = AdaDmg;
	}
	virtual void func(){
		cout << "I'm SuperMonster\n";
	}
	virtual ~SuperMonster(){}
};

struct SillySoldier : public Soldier{
	void func(){
		cout << "I'm SillySoldier,  my data is down below:\n"
			 << "HP : " << HP << "\tMP : " << MP << "\tAttack : " << Attack
			 << "\tDefence : " << Defence << "\n";
	}
	~SillySoldier(){
		cout << "Desturctor of SillySoldier\n";
	}
};
struct SillyMonster : public Monster{
	void func(){
		cout << "I'm SillyMonster, my data is down below:\n"
			 << "HP : " << HP << "\tAttack : " << Attack
			 << "\tDefence : " << Defence << "\n";
	}
	~SillyMonster(){
		cout << "Desturctor of SillyMonster\n";
	}
};
struct SillySuperMonster : public SuperMonster{
	void func(){
		cout << "I'm SillySuperMonster, my data is down below:\n"
			 << "HP : " << HP << "\tAttack : " << Attack
			 << "\tDefence : " << Defence << "\tAddationalDamage: " << AddationalDamage << "\n";
	}
	~SillySuperMonster(){
		cout << "Desturctor of SillySuperMonster\n";
	}
};


using AbstractEnemyFactory = AbstractFactory<TYPELIST(Soldier, Monster, SuperMonster)>;
using EasyLevelEnemyFactory = ConcreteFactory<AbstractEnemyFactory, OpNewFactoryUnit, TYPELIST(SillySoldier, SillyMonster, SillySuperMonster)>;
int main(){
	EasyLevelEnemyFactory easyFact;
	AbstractEnemyFactory* absFact = &easyFact;
	auto soldier = absFact->Create<Soldier>(100, 100, 100, 100);
	soldier->func();
	auto monster = absFact->Create<Monster>(100, 100, 100);
	monster->func();
	auto supermonster = absFact->Create<SuperMonster>(100, 100, 100, 100);
	supermonster->func();
	return 0;
}
