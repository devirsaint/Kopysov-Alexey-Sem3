#pragma once
#include "Matrix.h"

/**
* @brief Абстрактный класс задания, использующий матрицу и генератор
*/
class Exercise
{
public:
	/**
	* @brief Определяет, нужно ли заполнять матрицу при создании задания
	*/
	enum class FillMode
	{
		Generate,
		KeepValues
	};

protected:
	/**
	* @param matrix - матрица, с которой работает задание
	*/
	Matrix<int>& matrix;

	/**
	* @param generator - генератор для заполнения матрицы
	*/
	const Generator& generator;

public:
	/**
	* @brief Создаёт задание и заполняет матрицу, если это нужно
	* @param matrix - обрабатываемая матрица
	* @param generator - генератор для заполнения матрицы
	* @param mode - заполнить матрицу или сохранить её значения
	*/
	Exercise(Matrix<int>& matrix, const Generator& generator, const FillMode mode = FillMode::Generate);

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
