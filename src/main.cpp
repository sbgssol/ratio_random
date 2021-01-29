#include <iostream>
#include "random.h"
#include <random>
int main()
{
	Random::Core_Feature::setSeed(std::random_device{}());

	Random::Core<char> rnd;
	std::cout << std::showbase << std::boolalpha;
	std::cout << rnd.addObj('a'		, 10) << '\n';
	std::cout << rnd.addObj('b'		, 10) << '\n';
	std::cout << rnd.addObj('c'		, 10) << '\n';
	std::cout << rnd.addObj('d'		, 10) << '\n';

	std::cout << rnd.delObj('e') << '\n';
	std::cout << rnd.delObj('b') << '\n';
	std::cout << rnd.delObj('c') << '\n';

	std::cout << rnd.addObj('x', 20) << '\n';
	std::cout << rnd.addObj('x', 20) << '\n';
	std::cout << rnd.addObj('x', 20) << '\n';
	std::cout << rnd.addObj('x', 20) << '\n';
	std::cout << rnd.addObj('y', 40) << '\n';
	std::cout << rnd.addObj('z', 30) << '\n';

	std::cout << rnd.getObj() << '\n';

	rnd.dump();

	

	std::cin.get();
}