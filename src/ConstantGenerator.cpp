#include "ConstantGenerator.h"

ConstantGenerator::ConstantGenerator(int value) : value(value)
{
}

int ConstantGenerator::generate()
{
	return value;
}
