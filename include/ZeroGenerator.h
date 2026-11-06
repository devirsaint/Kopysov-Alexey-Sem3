#pragma once
#include "Generator.h"

/**
* @brief Класс генератора нулевых значений
*/
class ZeroGenerator : public Generator
{
public:
	/**
	* @brief Возвращает ноль для заполнения матрицы
	* @return Число 0
	*/
	int generate() override;
};
