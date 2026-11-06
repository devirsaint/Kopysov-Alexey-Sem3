#pragma once
#include "Matrix.h"

/**
* @brief Абстрактный класс задания, использующий матрицу и генератор
*/
class Exercise
{
protected:
	/**
	* @param matrix - матрица, с которой работает задание
	*/
	Matrix<int>& matrix;

	/**
	* @param generator - генератор для заполнения матрицы
	*/
	Generator& generator;

public:
	/**
	* @brief Конструктор класса, связывает задание с матрицей и генератором
	* @param matrix - обрабатываемая матрица
	* @param generator - генератор для заполнения матрицы
	*/
	Exercise(Matrix<int>& matrix, Generator& generator);

	/**
	* @brief Виртуальный деструктор класса
	*/
	virtual ~Exercise();

	/**
	* @brief Заполняет матрицу генератором, при ошибке сохраняет исходные данные
	*/
	void fill();

	/**
	* @brief Выполняет задание над текущей матрицей
	*/
	virtual void solve() = 0;
};
