#pragma once
#include "Generator.h"
#include <iosfwd>

/**
* @brief Класс генератора целых чисел из потока ввода
*/
class IStreamGenerator : public Generator
{
private:
	/**
	* @param in - ссылка на поток ввода
	*/
	std::istream& in;

public:
	/**
	* @brief Конструктор класса, задаёт поток для чтения чисел
	* @param in - поток ввода
	*/
	explicit IStreamGenerator(std::istream& in);

	/**
	* @brief Считывает целое число из потока с проверкой ввода
	* @return Считанное число типа int
	*/
	int generate() override;
};
