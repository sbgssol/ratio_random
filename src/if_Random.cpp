// if_Random.cpp
#include "if_Random.h"
#include "RandomLib.h"
#include <iostream>

class RandomFactor;


void setSeed(unsigned long t_seed)
{
	RandomCore::getInstance()->setSeed(t_seed);
}

unsigned long getSeed()
{
	return RandomCore::getInstance()->getSeed();
}

RandomWeight* getRndGen()
{
	auto ptr = new RandomWeight();
	return ptr;
}

bool setId(uint32_t t_id, uint32_t t_weight, RandomWeight * t_rnd_gen)
{
	return (t_rnd_gen->setId(t_id, t_weight));
}

bool delId(uint32_t t_id, RandomWeight * t_rnd_gen)
{
	return t_rnd_gen->delId(t_id);
}

uint32_t getId(RandomWeight * t_rnd_gen)
{
	return t_rnd_gen->getId();
}

uint32_t getWeight(uint32_t t_id, RandomWeight* t_rnd_gen)
{
	return t_rnd_gen->getWeight(t_id);
}

void releaseRndGen(RandomWeight * t_rnd_gen)
{
	delete t_rnd_gen;
}

RandomWeight * copyRndGen(RandomWeight * t_other)
{
	RandomWeight* ptr = new RandomWeight((*t_other));
	return ptr;
}

void dumpData(RandomWeight * t_rnd_gen)
{
	t_rnd_gen->dump();
}

template<typename T>
T rndInRange(T t_min, T t_max)
{
	return RandomCore::getInstance()->rndInRange<T>(t_min, t_max);
}


// Specialization -> rndInRange
template int32_t rndInRange<int32_t>(int32_t, int32_t);
template int64_t rndInRange<int64_t>(int64_t, int64_t);
template uint32_t rndInRange<uint32_t>(uint32_t, uint32_t);
template uint64_t rndInRange<uint64_t>(uint64_t, uint64_t);

