#include <iostream>
#include "random.h"
#include <random>
#include <numeric>


int main()
{
	RRND::Basic::set_seed(std::random_device{}());

	RRND::Core<> number_randomizer;
	number_randomizer.add(0, 10);
	number_randomizer.add(1, 20);
	number_randomizer.add(2, 10);
	number_randomizer.add(3, 30);
	number_randomizer.add(4, 10);
	number_randomizer.add(5, 40);
	number_randomizer.add(6, 10);
	number_randomizer.add(7, 50);
	number_randomizer.add(8, 10);
	number_randomizer.add(9, 60);
	std::vector<uint32_t> v(10, 0);
	for(uint32_t i = 0; i < 500000; ++i) {
		++v[number_randomizer.random()];
	}
	uint32_t sum = std::accumulate(v.begin(), v.end(), 0);
	for(uint32_t i = 0; i < 10; ++i) {
		std::cout << i << ": " << ((float)v[i] / sum) * 100 << " %" << '\n';
	}
}