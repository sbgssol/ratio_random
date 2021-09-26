#include <iostream>
#include <random>
#include <string>

#include "random.h"

struct A {
	int a;
  std::string str;

	friend std::ostream& operator<< (std::ostream& os, A const& obj) {
		os << obj.a << ": " << obj.str;
		return os;
	}
};

int main()
{
	RRND::Basic::set_seed(std::random_device{}());
	std::cout << std::showbase << std::hex << "Random seed: " << RRND::Basic::get_seed() << std::noshowbase << std::dec << '\n';

	RRND::Core<A> lst;
	lst.add(A{ 0, "Zero" }, 10);
	lst.add(A{ 1, "One" }, 50);
	lst.add(A{ 2, "Two" }, 10);
	lst.add(A{ 3, "Three" }, 10);
	lst.add(A{ 4, "Four" }, 10);

	lst.remove(3);
	lst.remove(2);

	lst.at(1).str = "First";
	lst.change_weight(1, 100);

	lst.dump([](A const & obj) { return std::to_string(obj.a) + " - " + obj.str; });


  int arr[] = { 0, 0, 0, 0, 0 };
	int const random_time = 100000;
	for(int i = 0; i < random_time; ++i) {
		++arr[lst.random().a];
	}

	for(int i = 0; i < 5; ++i) {
		float ratio = (float)(arr[i]) / random_time;
		std::cout << i << ": " << ratio * 100 << "%" << '\n';
	}
}
