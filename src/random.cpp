#include "random.h"
#include "core.h"
#include <iostream>

class RandomFactor;

void FBW_Random::Base::setSeed(uint32_t t_seed)
{
	_Core::getInstance()->setSeed(t_seed);
}

uint32_t FBW_Random::Base::getSeed()
{
	return _Core::getInstance()->getSeed();
}

_Weighted* FBW_Random::Base::Weighted_Feature::getRndGen()
{
	auto ptr = new _Weighted();
	return ptr;
}

bool FBW_Random::Base::Weighted_Feature::addId(uint32_t t_id, uint32_t t_weight, _Weighted * t_rnd_gen)
{
	return (t_rnd_gen->addId(t_id, t_weight));
}

bool FBW_Random::Base::Weighted_Feature::delId(uint32_t t_id, _Weighted * t_rnd_gen)
{
	return t_rnd_gen->delId(t_id);
}

uint32_t FBW_Random::Base::Weighted_Feature::getId(_Weighted * t_rnd_gen)
{
	return t_rnd_gen->getId();
}

uint32_t FBW_Random::Base::Weighted_Feature::getWeight(uint32_t t_id, _Weighted* t_rnd_gen)
{
	return t_rnd_gen->getWeight(t_id);
}

void FBW_Random::Base::Weighted_Feature::releaseRndGen(_Weighted * t_rnd_gen)
{
	delete t_rnd_gen;
}

_Weighted * FBW_Random::Base::Weighted_Feature::copyRndGen(_Weighted * t_other)
{
	_Weighted* ptr = new _Weighted((*t_other));
	return ptr;
}

void FBW_Random::Base::Weighted_Feature::dumpData(_Weighted * t_rnd_gen)
{
	t_rnd_gen->dump();
}

template<typename T>
T FBW_Random::Base::rndInRange(T t_min, T t_max)
{
	return _Core::getInstance()->rndInRange<T>(t_min, t_max);
}


// Specialization -> rndInRange
template int32_t	FBW_Random::Base::rndInRange<int32_t>(int32_t, int32_t);
template int64_t	FBW_Random::Base::rndInRange<int64_t>(int64_t, int64_t);
template uint32_t	FBW_Random::Base::rndInRange<uint32_t>(uint32_t, uint32_t);
template uint64_t	FBW_Random::Base::rndInRange<uint64_t>(uint64_t, uint64_t);

