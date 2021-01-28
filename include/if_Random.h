// if_Random.h
#pragma once
#include <cstdint>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>

class RandomCore;
class RandomWeight;

////// RANDOM CORE FEATURES //////
void 
setSeed(unsigned long t_seed);

unsigned long 
getSeed();

template<typename T>
T 
rndInRange(T t_min, T t_max);

////// RANDOM WEIGHT FEATURES //////
RandomWeight*
getRndGen();

void
releaseRndGen(RandomWeight* t_rnd_gen);

RandomWeight*
copyRndGen(RandomWeight* t_other);

bool
setId(uint32_t t_id, uint32_t t_weight, RandomWeight* t_rnd_gen);

bool
delId(uint32_t t_id, RandomWeight* t_rnd_gen);

uint32_t
getId(RandomWeight* t_rnd_gen);

void
dumpData(RandomWeight* t_rnd_gen);

uint32_t
getWeight(uint32_t t_id, RandomWeight* t_rnd_gen);


////// RANDOM ADAPTER //////
/* //// REQUIREMENTS FOR THE TARGET CLASS //// 
*	1. Operator need to be overloaded:
*		1.1: "==" -> comparision 
*		1.2" "<<" -> output stream*
*/
template<typename CLASS_T>
class RandomAdapter
{
public:
	explicit RandomAdapter();
	~RandomAdapter();
	explicit RandomAdapter<CLASS_T>(const RandomAdapter<CLASS_T>& t_other);
	RandomAdapter<CLASS_T>& operator=(const RandomAdapter<CLASS_T>& t_other);

	bool
	setObj(const CLASS_T& t_obj, uint32_t t_weight = 10);
	
	CLASS_T
	getObj() const;

	bool
	delObj(CLASS_T t_obj);

	void
	dump();

	bool
	valid() const;

	uint32_t
	countObj() const;

private:
	std::map<uint32_t, CLASS_T> m_objData;
	RandomWeight* m_rndGen;
	// id = (*end()) - 1
};

/* ///*****		END OF USER REFERENCE		*****/// */
//---------------------------------------------------//
//---------------------------------------------------//
//---------------------------------------------------//
// NOTE
// DEBUG












////// RANDOM ADAPTER FUNCTION DEFINITIONS //////
template<typename CLASS_T>
inline RandomAdapter<CLASS_T>::RandomAdapter(): m_rndGen(getRndGen())
{
}

template<typename CLASS_T>
inline RandomAdapter<CLASS_T>::~RandomAdapter()
{
	releaseRndGen(m_rndGen);
}

template<typename CLASS_T>
inline RandomAdapter<CLASS_T>::RandomAdapter(const RandomAdapter & t_other)
{
	m_objData = t_other.m_objData;
	m_rndGen = copyRndGen(t_other.m_rndGen);
}

template<typename CLASS_T>
inline RandomAdapter<CLASS_T> & RandomAdapter<CLASS_T>::operator=(const RandomAdapter<CLASS_T> & t_other)
{
	if (&t_other != this)
	{
		m_objData = t_other.m_objData;
		m_rndGen = copyRndGen(t_other.m_rndGen);
	}
	return *this;
}

template<typename CLASS_T>
inline bool RandomAdapter<CLASS_T>::setObj(const CLASS_T& t_obj, uint32_t t_weight)
{
	
	if (m_objData.size() == 0) // first time call
	{
		m_objData.insert({ 1, t_obj });
		setId(1, t_weight, m_rndGen);
		return true;
	}
	else
	{
		auto itr_existed = std::find_if(m_objData.begin(), m_objData.end(), [=](const std::pair<uint32_t, CLASS_T>& t_pair_obj) {
			return (t_pair_obj.second == t_obj);
		});
				
		if (itr_existed == m_objData.end())
		{
			uint32_t id = (*std::prev(m_objData.end())).first + 1;
			m_objData.insert({ id, t_obj });
			setId(id, t_weight, m_rndGen);
			return true;
		}
		else
		{
			//std::cout << "Object existed: " << (*itr_existed).second << " >< " << t_obj <<  '\n';
			std::cout << "WARNING: Object existed. "<<  '\n';
			std::cout << "Old= " << (*itr_existed).second << '\n';
			std::cout << "New= " << t_obj << " <= Ignored" <<'\n';
			return false;
		}
	}
}

template<typename CLASS_T>
inline CLASS_T RandomAdapter<CLASS_T>::getObj() const
{
	uint32_t id = getId(m_rndGen);
	return (*m_objData.find(id)).second;
}

template<typename CLASS_T>
inline bool RandomAdapter<CLASS_T>::delObj(CLASS_T t_obj)
{
	if (m_objData.size() == 0) // first time call
	{
		std::cout << "Nothing to delete." << '\n';
		return false;
	}
	else
	{
		auto itr_existed = std::find_if(m_objData.begin(), m_objData.end(), [=](const std::pair<uint32_t, CLASS_T>& t_pair_obj) {
			return (t_pair_obj.second == t_obj);
		});

		// 2. Delete
		if (itr_existed != m_objData.end())
		{
			uint32_t id = (*itr_existed).first;
			m_objData.erase(id);
			delId(id, m_rndGen);			
			return true;
		}
		else
		{
			std::cout << "Not found object." << '\n';
			return false;
		}
	}
	return false;
}

template<typename CLASS_T>
inline void RandomAdapter<CLASS_T>::dump()
{
	std::vector<std::pair<CLASS_T, uint32_t>> dump_data{};
	for (auto itr = m_objData.begin(); itr != m_objData.end(); ++itr)
	{
		dump_data.push_back({ (*itr).second, getWeight((*itr).first, m_rndGen) });
	}
	
	std::cout << "\t"<< "==== DATA objects ====" << '\n';
	std::cout << "\t "<< "WEIGHT" << " <-> " << "OBJECTS" << '\n';
	for (auto itr = dump_data.begin(); itr != dump_data.end(); ++itr)
	{
		std::cout << "\t  " << std::right << std::setw(5) << std::setfill(' ') << (*itr).second << " <-> " << (*itr).first << '\n';
	}
	std::cout << '\n';
	dumpData(m_rndGen);
}

template<typename CLASS_T>
inline bool RandomAdapter<CLASS_T>::valid() const
{
	return (m_objData.size());
}

template<typename CLASS_T>
inline uint32_t RandomAdapter<CLASS_T>::countObj() const
{
	return (uint32_t)m_objData.size();
}

