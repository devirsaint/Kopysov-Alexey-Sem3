#pragma once

#include "Generator.h"
#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

/**
* @brief Шаблонный класс двумерного массива
*/
template<typename Type>
class Matrix
{
private:
	/**
	* @param rows - количество строк матрицы
	*/
	std::size_t rows;

	/**
	* @param columns - количество столбцов матрицы
	*/
	std::size_t columns;

	/**
	* @param data - элементы матрицы, расположенные построчно
	*/
	std::vector<Type> data;

public:
	/**
	* @brief Конструктор класса по умолчанию, задаёт пустую матрицу
	*/
	Matrix();

	/**
	* @brief Конструктор класса, задаёт размеры матрицы и заполняет её значением
	* @param rows - количество строк матрицы
	* @param columns - количество столбцов матрицы
	* @param value - значение каждого элемента матрицы
	*/
	Matrix(std::size_t rows, std::size_t columns, const Type& value = Type());

	/**
	* @brief Конструктор класса, задаёт матрицу по списку инициализации
	* @param values - список строк одинаковой длины
	*/
	Matrix(std::initializer_list<std::initializer_list<Type>> values);

	/**
	* @brief Конструктор копирования, создаёт независимую копию матрицы
	* @param other - копируемая матрица
	*/
	Matrix(const Matrix& other);

	/**
	* @brief Конструктор перемещения, оставляет исходную матрицу пустой
	* @param other - матрица, данные которой перемещаются
	*/
	Matrix(Matrix&& other) noexcept;

	/**
	* @brief Деструктор класса
	*/
	~Matrix();

	/**
	* @brief Оператор присваивания матрицы с копированием данных
	* @param other - копируемая матрица
	* @return Ссылка на текущую матрицу
	*/
	Matrix& operator = (const Matrix& other);

	/**
	* @brief Оператор присваивания матрицы с перемещением данных
	* @param other - матрица, данные которой перемещаются
	* @return Ссылка на текущую матрицу
	*/
	Matrix& operator = (Matrix&& other) noexcept;

	/**
	* @brief Получает количество строк матрицы
	* @return Количество строк матрицы
	*/
	std::size_t getRows() const noexcept;

	/**
	* @brief Получает количество столбцов матрицы
	* @return Количество столбцов матрицы
	*/
	std::size_t getColumns() const noexcept;

	/**
	* @brief Проверяет, пуста ли матрица
	* @return true, если в матрице нет элементов, иначе false
	*/
	bool empty() const noexcept;

	/**
	* @brief Получает элемент матрицы по строке и столбцу с проверкой границ
	* @param row - индекс строки, начиная с нуля
	* @param column - индекс столбца, начиная с нуля
	* @return Ссылка на элемент матрицы
	*/
	Type& at(std::size_t row, std::size_t column);

	/**
	* @brief Получает элемент константной матрицы по строке и столбцу с проверкой границ
	* @param row - индекс строки, начиная с нуля
	* @param column - индекс столбца, начиная с нуля
	* @return Константная ссылка на элемент матрицы
	*/
	const Type& at(std::size_t row, std::size_t column) const;

	/**
	* @brief Оператор получения элемента матрицы по индексу
	* @param index - индекс элемента при построчном обходе: row * columns + column
	* @return Ссылка на элемент матрицы
	*/
	Type& operator [] (std::size_t index);

	/**
	* @brief Оператор получения элемента матрицы по индексу без возможности изменения
	* @param index - индекс элемента при построчном обходе: row * columns + column
	* @return Константная ссылка на элемент матрицы
	*/
	const Type& operator [] (std::size_t index) const;

	/**
	* @brief Заполняет матрицу генератором, при ошибке сохраняет исходные данные
	* @param generator - генератор значений для заполнения матрицы
	*/
	void fill(Generator& generator);

	/**
	* @brief Преобразует матрицу в строку
	* @return Строковое представление матрицы
	*/
	std::string toString() const;

	/**
	* @brief Вставляет столбец в матрицу перед указанным индексом
	* @param index - индекс вставки от нуля до количества столбцов включительно
	* @param values - значения столбца, по одному для каждой строки матрицы
	*/
	void insertColumn(std::size_t index, const std::vector<Type>& values);

	/**
	* @brief Оператор добавления строки в конец матрицы
	* @param row - строка, добавляемая в конец матрицы
	* @return Ссылка на текущую матрицу
	*/
	Matrix& operator << (const std::vector<Type>& row);

	/**
	* @brief Оператор извлечения первой строки матрицы
	* @param row - вектор, в который записывается извлечённая строка
	* @return Ссылка на текущую матрицу
	*/
	Matrix& operator >> (std::vector<Type>& row);
};

// Подключаем реализацию методов класса Matrix
#include "../src/Matrix.cpp"
