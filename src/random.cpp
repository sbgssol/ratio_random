#include "random.h"
#include "core.h"
#include <iostream>

class RandomFactor;

void Random::Core_Feature::setSeed(uint32_t t_seed)
{
	_Core::getInstance()->setSeed(t_seed);
}

uint32_t Random::Core_Feature::getSeed()
{
	return _Core::getInstance()->getSeed();
}

_Weighted* Random::Weighted_Feature::getRndGen()
{
	auto ptr = new _Weighted();
	return ptr;
}

bool Random::Weighted_Feature::addId(uint32_t t_id, uint32_t t_weight, _Weighted * t_rnd_gen)
{
	return (t_rnd_gen->addId(t_id, t_weight));
}

bool Random::Weighted_Feature::delId(uint32_t t_id, _Weighted * t_rnd_gen)
{
	return t_rnd_gen->delId(t_id);
}

uint32_t Random::Weighted_Feature::getId(_Weighted * t_rnd_gen)
{
	return t_rnd_gen->getId();
}

uint32_t Random::Weighted_Feature::getWeight(uint32_t t_id, _Weighted* t_rnd_gen)
{
	return t_rnd_gen->getWeight(t_id);
}

void Random::Weighted_Feature::releaseRndGen(_Weighted * t_rnd_gen)
{
	delete t_rnd_gen;
}

_Weighted * Random::Weighted_Feature::copyRndGen(_Weighted * t_other)
{
	_Weighted* ptr = new _Weighted((*t_other));
	return ptr;
}

void Random::Weighted_Feature::dumpData(_Weighted * t_rnd_gen)
{
	t_rnd_gen->dump();
}

template<typename T>
T Random::Core_Feature::rndInRange(T t_min, T t_max)
{
	return _Core::getInstance()->rndInRange<T>(t_min, t_max);
}


// Specialization -> rndInRange
template int32_t	Random::Core_Feature::rndInRange<int32_t>(int32_t, int32_t);
template int64_t	Random::Core_Feature::rndInRange<int64_t>(int64_t, int64_t);
template uint32_t	Random::Core_Feature::rndInRange<uint32_t>(uint32_t, uint32_t);
template uint64_t	Random::Core_Feature::rndInRange<uint64_t>(uint64_t, uint64_t);

