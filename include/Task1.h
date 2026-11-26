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
	* @param mode - заполнить матрицу или сохранить её значения
	*/
	Task1(Matrix<int>& matrix, const Generator& generator, const FillMode mode = FillMode::Generate);

	/**
	* @brief Заменяет все нечётные элементы каждой строки нулями
	*/
	void solve() override;
};
