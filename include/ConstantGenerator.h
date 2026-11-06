#pragma once
#include "Generator.h"

/**
* @brief Класс генератора постоянного значения
*/
class ConstantGenerator : public Generator
{
private:
	/**
	* @param value - значение, возвращаемое генератором
	*/
	int value;

public:
	/**
	* @brief Конструктор класса, задаёт постоянное значение генератора
	* @param value - значение для заполнения матрицы
	*/
	explicit ConstantGenerator(int value);

	/**
	* @brief Возвращает постоянное значение генератора
	* @return Заданное целое число
	*/
	int generate() override;
};
