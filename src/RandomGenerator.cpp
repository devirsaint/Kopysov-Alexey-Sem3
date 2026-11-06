#include "RandomGenerator.h"
#include <stdexcept>

RandomGenerator::RandomGenerator(int min, int max)
	: RandomGenerator(min, max, std::random_device{}())
{
}

RandomGenerator::RandomGenerator(int min, int max, unsigned int seed)
	: generator(seed)
{
	if (min > max)
	{
		throw std::invalid_argument("Минимум диапазона больше максимума");
	}
	distribution = std::uniform_int_distribution<int>(min, max);
}

int RandomGenerator::generate()
{
	return distribution(generator);
}
