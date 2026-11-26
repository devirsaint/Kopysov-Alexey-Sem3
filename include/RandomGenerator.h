#pragma once
#include "Generator.h"
#include <random>

/**
* @brief Класс генератора случайных целых чисел
*/
class RandomGenerator : public Generator
{
private:
	/**
	* @param distribution - равномерное распределение целых чисел в заданном диапазоне
	*/
	mutable std::uniform_int_distribution<int> distribution;

	/**
	* @param generator - хранит состояние последовательности случайных чисел
	*/
	mutable std::mt19937 generator;

public:
	/**
	* @brief Конструктор класса, задаёт диапазон случайных чисел
	* @param min - минимальное возможное число
	* @param max - максимальное возможное число
	*/
	RandomGenerator(const int min, const int max);

	/**
	* @brief Конструктор класса, позволяет повторить те же случайные числа
	* @param min - минимальное возможное число
	* @param max - максимальное возможное число
	* @param seed - начальное значение генератора
	*/
	RandomGenerator(const int min, const int max, const unsigned int seed);

	/**
	* @brief Получает случайное целое число из заданного диапазона
	* @return Случайное число типа int
	*/
	int generate() const override;
};
