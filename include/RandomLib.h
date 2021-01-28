// RandomLib.h
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
class RandomCore
{
public:
	template<typename T>
	T				rndInRange(T t_min, T t_max);

	static	
	RandomCore*		getInstance();
	void			setSeed(unsigned long t_seed);
	unsigned long	getSeed();
	bool			isValid();

private:
	RandomCore();
	~RandomCore();
	RandomCore(const RandomCore& other)				= delete;
	RandomCore& operator=(const RandomCore& other)	= delete;

	static 
	RandomCore*		m_instance; // #data_member: the random core object
	std::mt19937	m_engine;	// #data_member: the engine
	unsigned long	m_seed;		// #data_member: seed of the engine
	bool			m_valid;	// #data_member: seed is set or not
};

////// ------ RANDOM WEIGHT ------ //////
class RandomWeight
{
public:
	bool		setId(uint32_t t_id, uint32_t t_weight);
	bool		delId(uint32_t t_id);
	uint32_t	getId();
	uint32_t	getWeight(uint32_t t_id);
	void		dump();

	explicit RandomWeight();
	explicit RandomWeight(const RandomWeight& other);
	RandomWeight& operator=(RandomWeight& other) = delete;
	~RandomWeight();

	std::map<uint32_t, uint32_t>	m_data;
	uint32_t						m_totalWeight;
};

template<typename T>
inline T RandomCore::rndInRange(T t_min, T t_max)
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
		std::cout << "(" << typeid(t_min).name() << ")["<< t_min << " : " << t_max << "]" << '\n';
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