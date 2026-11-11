#include "IStreamGenerator.h"
#include <charconv>
#include <istream>
#include <stdexcept>
#include <string>

IStreamGenerator::IStreamGenerator(std::istream& in) : in(in)
{
}

int IStreamGenerator::generate()
{
	int value = 0;
	std::string token;
	if (!(in >> token))
	{
		throw std::runtime_error("Ошибка ввода целого числа");
	}
	const char* begin = token.data();
	const char* end = begin + token.size();
	// Пропускаем знак + перед числом, так как from_chars его не принимает
	if (*begin == '+')
	{
		++begin;
		if (begin == end || *begin < '0' || *begin > '9')
		{
			throw std::runtime_error("Ошибка ввода целого числа");
		}
	}
	const auto result = std::from_chars(begin, end, value);
	if (result.ec != std::errc() || result.ptr != end)
	{
		throw std::runtime_error("Ошибка ввода целого числа");
	}
	return value;
}
