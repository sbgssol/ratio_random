#include "core.h"
#include <iostream>

_Core* _Core::m_instance{ nullptr };

_Core * _Core::getInstance()
{
	if (m_instance == nullptr)
	{
		m_instance = new _Core();
	}
	return m_instance;
}

void _Core::setSeed(uint32_t t_seed)
{
	if (m_valid == false)
	{
		m_valid = true;
		m_seed = t_seed;
		m_engine.seed(m_seed);
	}
	else
	{
		std::cout << "Current seed: " << std::showbase << std::hex << m_seed << std::dec << ", new seed has no effect." << '\n';
	}
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

bool _Weighted::addId(uint32_t t_id, uint32_t t_weight)
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

bool _Weighted::delId(uint32_t t_id)
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

uint32_t _Weighted::getId()
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

uint32_t _Weighted::getWeight(uint32_t t_id)
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

void _Weighted::dump()
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

_Weighted::_Weighted() : m_data({}), m_totalWeight(0)
{}

_Weighted::_Weighted(const _Weighted & other) : m_data(other.m_data), m_totalWeight(other.m_totalWeight)
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


_Weighted::~_Weighted()
{}
