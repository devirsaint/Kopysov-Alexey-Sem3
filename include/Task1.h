#pragma once
#include "Exercise.h"

/**
* @brief Класс первого задания варианта 14
*/
class Task1 : public Exercise
{
public:
	/**
	* @brief Конструктор класса, связывает первое задание с матрицей и генератором
	* @param matrix - обрабатываемая матрица
	* @param generator - генератор для заполнения матрицы
	*/
	Task1(Matrix<int>& matrix, Generator& generator);

	/**
	* @brief Заменяет все нечётные элементы каждой строки нулями
	*/
	void solve() override;
};
