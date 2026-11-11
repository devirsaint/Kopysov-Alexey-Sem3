#pragma once

#include <cstddef>

/**
* @brief Класс для доступа к элементам строки матрицы
*/
template<typename Type>
class MatrixRow
{
private:
	/**
	* @param data - указатель на первый элемент строки
	*/
	Type* data;

	/**
	* @param columns - количество элементов строки
	*/
	std::size_t columns;

public:
	/**
	* @brief Конструктор класса, задаёт строку матрицы без копирования элементов
	* @param data - указатель на первый элемент строки
	* @param columns - количество элементов строки
	*/
	MatrixRow(Type* data, std::size_t columns);

	/**
	* @brief Оператор доступа к элементу строки с проверкой границ
	* @param column - индекс столбца, начиная с нуля
	* @return Ссылка на элемент строки
	*/
	Type& operator [] (std::size_t column) const;
};

#include "../src/MatrixRow.cpp"
