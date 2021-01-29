#pragma once
#include <cstdint>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

class _Core;
class _Weighted;

namespace Random
{
	// Client code uses this type to:
	// 1. Set random seed
	// 2. Get random seed
	// 3. Random a number in a specific range
	namespace Core_Feature
	{
		void		setSeed(uint32_t t_seed);
		uint32_t	getSeed();
		template<typename T>
		T			rndInRange(T t_min, T t_max);
	}
	/* === CLASS_T requirements ===*/
	/* 1. To random CLASS_T objects, CLASS_T must overload the operator=*/
	/* 2. To use dump function, CLASS_T must overload the operator<<*/
	template<typename CLASS_T>
	class Core
	{
	public:
		~Core();
		explicit	Core();
		explicit	Core<CLASS_T>(const Core<CLASS_T>& t_other);

		bool		addObj(const CLASS_T& t_obj, uint32_t t_weight = 10);
		CLASS_T		getObj() const;
		bool		delObj(CLASS_T t_obj);
		void		dump();
		bool		valid() const;
		uint32_t	countObj() const;

	private:
		std::map<uint32_t, CLASS_T> m_objData;
		_Weighted*					m_rndGen;
	};

	// ==== IMPORTANT ====
	// Client code cannot use this type.
	class Weighted_Feature
	{
		// Inaccessible from client codes
		static _Weighted*	getRndGen();
		static void			releaseRndGen(_Weighted* t_rnd_gen);
		static _Weighted*	copyRndGen(_Weighted* t_other);
		static bool			addId(uint32_t t_id, uint32_t t_weight, _Weighted* t_rnd_gen);
		static bool			delId(uint32_t t_id, _Weighted* t_rnd_gen);
		static uint32_t		getId(_Weighted* t_rnd_gen);
		static void			dumpData(_Weighted* t_rnd_gen);
		static uint32_t		getWeight(uint32_t t_id, _Weighted* t_rnd_gen);

		template<class T>
		friend class Core;
	};

}
















/*====== Random Core implementation ======*/
template<typename CLASS_T>
inline Random::Core<CLASS_T>::Core() : m_rndGen(Random::Weighted_Feature::getRndGen())
{}

template<typename CLASS_T>
inline Random::Core<CLASS_T>::~Core()
{
	Random::Weighted_Feature::releaseRndGen(m_rndGen);
}

template<typename CLASS_T>
inline Random::Core<CLASS_T>::Core(const Core & t_other)
	: m_objData(t_other.m_objData), m_rndGen(Random::Weighted_Feature::copyRndGen(t_other.m_rndGen))
{}

template<typename CLASS_T>
inline bool Random::Core<CLASS_T>::addObj(const CLASS_T& t_obj, uint32_t t_weight)
{

	uint32_t id = ((m_objData.size()) ? ((*std::prev(m_objData.end())).first) : (1));

	bool object_exists = std::find_if(m_objData.begin(), m_objData.end(), [=](const std::pair<uint32_t, CLASS_T>& t_pair_obj)
	{
		return (t_pair_obj.second == t_obj);
	}) != m_objData.end();

	if (object_exists == false)
	{
		uint32_t id = ((m_objData.size()) ? ((*std::prev(m_objData.end())).first + 1) : (1));
		m_objData.insert({ id, t_obj });
		Random::Weighted_Feature::addId(id, t_weight, m_rndGen);
	}
	return object_exists;

}

template<typename CLASS_T>
inline CLASS_T Random::Core<CLASS_T>::getObj() const
{
	return (*m_objData.find(Random::Weighted_Feature::getId(m_rndGen))).second;
}

template<typename CLASS_T>
inline bool Random::Core<CLASS_T>::delObj(CLASS_T t_obj)
{
	if (m_objData.size() == 0) // first time call
	{
		//std::cout << "Nothing to delete." << '\n';
		return false;
	}
	else
	{
		auto itr_existed = std::find_if(m_objData.begin(), m_objData.end(), [=](const std::pair<uint32_t, CLASS_T>& t_pair_obj)
		{
			return (t_pair_obj.second == t_obj);
		});

		// 2. Delete
		if (itr_existed != m_objData.end())
		{
			uint32_t id = (*itr_existed).first;
			m_objData.erase(id);
			Random::Weighted_Feature::delId(id, m_rndGen);
			return true;
		}
		else
		{
			//std::cout << "Not found object." << '\n';
			return false;
		}
	}
	return false;
}

template<typename CLASS_T>
inline void Random::Core<CLASS_T>::dump()
{
	std::vector<std::pair<CLASS_T, uint32_t>> dump_data{};
	for (auto itr = m_objData.begin(); itr != m_objData.end(); ++itr)
	{
		dump_data.push_back({ (*itr).second, Random::Weighted_Feature::getWeight((*itr).first, m_rndGen) });
	}

	std::cout << "\t" << "==== DATA objects ====" << '\n';
	std::cout << "\t " << "WEIGHT" << " <-> " << "OBJECTS" << '\n';
	for (auto itr = dump_data.begin(); itr != dump_data.end(); ++itr)
	{
		std::cout << "\t  " << std::right << std::setw(5) << std::setfill(' ') << (*itr).second << " <-> " << (*itr).first << '\n';
	}
	std::cout << '\n';
	Random::Weighted_Feature::dumpData(m_rndGen);
}

template<typename CLASS_T>
inline bool Random::Core<CLASS_T>::valid() const
{
	return (m_objData.size());
}

template<typename CLASS_T>
inline uint32_t Random::Core<CLASS_T>::countObj() const
{
	return (uint32_t)m_objData.size();
}
