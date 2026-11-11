#pragma once
#include "Exercise.h"

/**
* @brief Класс второго задания варианта 14
*/
class Task2 : public Exercise
{
public:
	/**
	* @brief Конструктор класса, связывает второе задание с матрицей и генератором
	* @param matrix - обрабатываемая матрица
	* @param generator - генератор для заполнения матрицы
	*/
	Task2(Matrix<int>& matrix, Generator& generator);

	/**
	* @brief Вставляет копию первого столбца после каждого исходного столбца с нулём
	*/
	void solve() override;
};
