#include "random.h"
#include <iostream>
#include <random>
#include <exception>
#include <cstdint>
#include <typeinfo>
#include <map>
#include <iomanip>
#include <typeinfo>

class _Core
{
public:
	template<typename T>
	T				rndInRange(T t_min, T t_max);
	static _Core*	getInstance();
	bool			setSeed(uint32_t t_seed);
	uint32_t		getSeed();
	bool			isValid();

private:
	_Core();
	~_Core();
	_Core(const _Core& other) = delete;
	_Core& operator=(const _Core& other) = delete;

	static
		_Core*		m_instance; // #data_member: the random core object
	std::mt19937	m_engine;	// #data_member: the engine
	uint32_t	m_seed;		// #data_member: seed of the engine
	bool			m_valid;	// #data_member: seed is set or not
};

class FBW_Random::Base::Weighted_Feature::_Weighted
{
public:
	bool		addId(uint32_t t_id, uint32_t t_weight);
	bool		delId(uint32_t t_id);
	uint32_t	getId();
	uint32_t	getWeight(uint32_t t_id);
	void		dump();

	explicit _Weighted();
	explicit _Weighted(const _Weighted& other);
	_Weighted& operator=(_Weighted& other) = delete;
	~_Weighted();

	std::map<uint32_t, uint32_t>	m_data;
	uint32_t						m_totalWeight;
};

_Core* _Core::m_instance{ nullptr };

_Core * _Core::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new _Core();
	}
	return m_instance;
}

bool _Core::setSeed(uint32_t t_seed)
{
	if (m_valid == false)
	{
		m_valid = true;
		m_seed = t_seed;
		m_engine.seed(m_seed);
		return true;
	}

	return (m_valid == false);
}

uint32_t _Core::getSeed()
{
	try
	{
		if (m_valid != false)
		{
			return m_seed;
		}
		else
		{
			throw std::invalid_argument{ "ERROR: No seed is specified./." };
		}
	}
	catch (const std::runtime_error& exp)
	{
		std::cout << exp.what() << '\n';
		// std::cout << "Press ENTER to exit..." << '\n';
		// std::cin.get();
		exit(EXIT_FAILURE);
	}
}

bool _Core::isValid()
{
	return m_valid;
}

_Core::_Core() :m_seed(0), m_valid(false)
{}

_Core::~_Core()
{}

bool FBW_Random::Base::setSeed(uint32_t t_seed)
{
	return _Core::getInstance()->setSeed(t_seed);
}

uint32_t FBW_Random::Base::getSeed()
{
	return _Core::getInstance()->getSeed();
}

FBW_Random::Base::Weighted_Feature::_Weighted* FBW_Random::Base::Weighted_Feature::getRndGen()
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

FBW_Random::Base::Weighted_Feature::_Weighted * FBW_Random::Base::Weighted_Feature::copyRndGen(_Weighted * t_other)
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

template<typename T>
inline T _Core::rndInRange(T t_min, T t_max)
{
	try
	{
		if (m_valid != false)
		{
			if (t_min < t_max)
			{
				return std::uniform_int_distribution<T>(t_min, t_max)(m_engine);
			}
			else if (t_min == t_max)
			{
				return t_min;
			}
			else
			{
				throw std::invalid_argument{ "ERROR: Wrong range./." };
			}
		}
		else
		{
			throw std::invalid_argument{ "ERROR: No seed is specified./." };
			// Throw run time error: There is no seed
		}
	}
	catch (const std::invalid_argument& exp)
	{
		std::cout << exp.what() << '\n';
		std::cout << "(" << typeid(t_min).name() << ")[" << t_min << " : " << t_max << "]" << '\n';
		// std::cout << "Press ENTER to exit..." << '\n';
		// std::cin.get();
		exit(EXIT_FAILURE);
	}
	catch (const std::runtime_error& exp)
	{
		std::cout << exp.what() << '\n';
		// std::cout << "Press ENTER to exit..." << '\n';
		// std::cin.get();
		exit(EXIT_FAILURE);
	}
}

template int32_t	_Core::rndInRange<int32_t>(int32_t, int32_t);
template int64_t	_Core::rndInRange<int64_t>(int64_t, int64_t);
template uint32_t	_Core::rndInRange<uint32_t>(uint32_t, uint32_t);
template uint64_t	_Core::rndInRange<uint64_t>(uint64_t, uint64_t);


bool FBW_Random::Base::Weighted_Feature::_Weighted::addId(uint32_t t_id, uint32_t t_weight)
{
	if (t_weight > 0)
	{
		auto result = m_data.insert({ t_id, t_weight });
		if (result.second)
		{
			m_totalWeight += t_weight;
		}
		return result.second;
	}
	else
	{
		return false;
	}
}

bool FBW_Random::Base::Weighted_Feature::_Weighted::delId(uint32_t t_id)
{
	auto find_result = m_data.find(t_id);
	auto removed_weight = (find_result != m_data.end()) ? ((*find_result).second) : 0;
	auto result = m_data.erase(t_id);
	if (result)
	{
		m_totalWeight -= removed_weight;
	}
	return result;
}

uint32_t FBW_Random::Base::Weighted_Feature::_Weighted::getId()
{
	/*try
	{*/
	if (m_data.size() > 0)
	{
		uint32_t random_weight = _Core::getInstance()->rndInRange<uint32_t>(1, m_totalWeight);
		uint32_t current_weight = 0;
		uint32_t id = 0;
		for (auto itr = m_data.begin(); itr != m_data.end(); ++itr)
		{
			current_weight += (*itr).second;
			if (current_weight >= random_weight)
			{
				id = (*itr).first;
				break;
			}
		}
		return id;
	}
	else
	{
		throw std::invalid_argument{ "ERROR: Empty object data." };
	}
	/*}
	catch (const std::runtime_error& exp)
	{
	std::cout << exp.what() << '\n';
	// std::cout << "Press ENTER to exit..." << '\n';
	// std::cin.get();
	exit(EXIT_FAILURE);
	}*/
}

uint32_t FBW_Random::Base::Weighted_Feature::_Weighted::getWeight(uint32_t t_id)
{
	try
	{
		auto find_result = m_data.find(t_id);
		auto target_weight = (find_result != m_data.end()) ? ((*find_result).second) : std::string::npos;
		if (target_weight != std::string::npos)
		{
			return (uint32_t)target_weight;
		}
		else
		{
			throw std::invalid_argument{ "ERROR: Not found object." };
		}
	}
	catch (const std::runtime_error& exp)
	{
		std::cout << exp.what() << '\n';
		// std::cout << "Press ENTER to exit..." << '\n';
		// std::cin.get();
		exit(EXIT_FAILURE);
	}

}

void FBW_Random::Base::Weighted_Feature::_Weighted::dump()
{
	std::cout << "\t==== DATA table ====" << '\n';
	std::cout << "\t     " << "ID" << " <-> " << "WEIGHT" << '\n';
	for (auto itr = m_data.begin(); itr != m_data.end(); ++itr)
	{
		std::cout << "\t" << std::setw(7) << std::setfill(' ') << (*itr).first << " <-> " << (*itr).second << '\n';
	}
	std::cout << "\t       " << "-----" << '\n';
	std::cout << "\t-> Total W: " << m_totalWeight << '\n';
	std::cout << '\n';
}

FBW_Random::Base::Weighted_Feature::_Weighted::_Weighted() : m_data({}), m_totalWeight(0)
{}

FBW_Random::Base::Weighted_Feature::_Weighted::_Weighted(const _Weighted & other) : m_data(other.m_data), m_totalWeight(other.m_totalWeight)
{

}

//RandomWeight & RandomWeight::operator=(RandomWeight & other)
//{
//	if (other.m_data.empty() == false)
//	{
//		std::copy(other.m_data.begin(), other.m_data.end(), this->m_data.begin());
//		this->m_data = other.m_data;
//	}
//	else
//	{
//		this->m_data.clear();
//		this->m_totalWeight = 0;
//	}
//
//	return *this;
//}


FBW_Random::Base::Weighted_Feature::_Weighted::~_Weighted()
{}
