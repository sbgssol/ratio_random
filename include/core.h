#pragma once
#include <random>
#include <exception>
#include <cstdint>
#include <typeinfo>
#include <iostream>
#include <map>
#include <iomanip>
#include <typeinfo>

////// ------ RANDOM CORE ------ //////
class _Core
{
public:
	template<typename T>
	T				rndInRange(T t_min, T t_max);

	static
		_Core*		getInstance();
	void			setSeed(uint32_t t_seed);
	uint32_t	getSeed();
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

////// ------ RANDOM WEIGHT ------ //////
class _Weighted
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

