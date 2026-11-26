#include "ConstantGenerator.h"

ConstantGenerator::ConstantGenerator(const int value) : value(value) {}

int ConstantGenerator::generate() const
{
	return value;
}
