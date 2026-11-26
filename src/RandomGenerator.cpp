#include "RandomGenerator.h"
#include <stdexcept>

RandomGenerator::RandomGenerator(const int min, const int max) : RandomGenerator(min, max, std::random_device{}()) {}

RandomGenerator::RandomGenerator(const int min, const int max, const unsigned int seed)
	: generator(seed)
{
	if (min > max)
	{
		throw std::invalid_argument("Минимум диапазона больше максимума");
	}
	distribution = std::uniform_int_distribution<int>(min, max);
}

int RandomGenerator::generate() const
{
	return distribution(generator);
}
