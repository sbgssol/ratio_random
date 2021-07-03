#include <iostream>
#include "random.h"
#include <random>
#include <numeric>


int main()
{
	RRND::Basic::set_seed(std::random_device{}());

	RRND::Core<> rnd;
	rnd.add(0,  5);
	rnd.add(1, 10);
	rnd.add(2, 40);
	rnd.add(3, 10);
	rnd.add(4, 10);
	rnd.dump();

	rnd.remove(0);
	rnd.remove(3);
	rnd.dump();


	std::vector<uint32_t> v{ 0,0,0,0,0 };

	for(uint32_t i = 0; i < 10000; ++i) {
		auto pos = rnd.random();
	  ++v.at(pos);
	}

	uint32_t sum = std::accumulate(v.begin(), v.end(), 0);
	for(uint32_t i = 0; i < v.size(); ++i) {
		std::cout << i << ": " << (((float) v[i]/ sum) * 100) << " %" << '\n';
	}
	
}