#ifndef MATRIX_IMPLEMENTATION
#define MATRIX_IMPLEMENTATION

#include "Matrix.h"
#include <limits>
#include <stdexcept>
#include <utility>

template<typename Type>
Matrix<Type>::Matrix() : rows(0), columns(0)
{
}

template<typename Type>
Matrix<Type>::Matrix(std::size_t rows, std::size_t columns, const Type& value)
	: rows(rows), columns(columns)
{
	if (columns != 0 && rows > std::numeric_limits<std::size_t>::max() / columns)
	{
		throw std::length_error("Слишком большой размер матрицы");
	}
	data.assign(rows * columns, value);
}

template<typename Type>
Matrix<Type>::Matrix(std::initializer_list<std::initializer_list<Type>> values)
	: Matrix(values.size(), values.size() == 0 ? 0 : values.begin()->size())
{
	std::size_t index = 0;
	for (const auto& row : values)
	{
		if (row.size() != columns)
		{
			throw std::invalid_argument("Строки матрицы должны иметь одинаковую длину");
		}
		for (const auto& value : row)
		{
			data[index++] = value;
		}
	}
}

template<typename Type>
Matrix<Type>::Matrix(const Matrix& other) = default;

template<typename Type>
Matrix<Type>::Matrix(Matrix&& other) noexcept
	: rows(other.rows), columns(other.columns), data(std::move(other.data))
{
	other.rows = 0;
	other.columns = 0;
	other.data.clear();
}

template<typename Type>
Matrix<Type>::~Matrix() = default;

template<typename Type>
Matrix<Type>& Matrix<Type>::operator = (const Matrix& other)
{
	if (this != &other)
	{
		Matrix copy(other);
		*this = std::move(copy);
	}
	return *this;
}

template<typename Type>
Matrix<Type>& Matrix<Type>::operator = (Matrix&& other) noexcept
{
	if (this != &other)
	{
		data = std::move(other.data);
		rows = other.rows;
		columns = other.columns;
		other.rows = 0;
		other.columns = 0;
		other.data.clear();
	}
	return *this;
}

template<typename Type>
std::size_t Matrix<Type>::getRows() const noexcept
{
	return rows;
}

template<typename Type>
std::size_t Matrix<Type>::getColumns() const noexcept
{
	return columns;
}

template<typename Type>
bool Matrix<Type>::empty() const noexcept
{
	return data.empty();
}

template<typename Type>
Type& Matrix<Type>::at(std::size_t row, std::size_t column)
{
	if (row >= rows || column >= columns)
	{
		throw std::out_of_range("Выход за границы матрицы");
	}
	return data[row * columns + column];
}

template<typename Type>
const Type& Matrix<Type>::at(std::size_t row, std::size_t column) const
{
	if (row >= rows || column >= columns)
	{
		throw std::out_of_range("Выход за границы матрицы");
	}
	return data[row * columns + column];
}

template<typename Type>
Type& Matrix<Type>::operator [] (std::size_t index)
{
	return data.at(index);
}

template<typename Type>
const Type& Matrix<Type>::operator [] (std::size_t index) const
{
	return data.at(index);
}

template<typename Type>
void Matrix<Type>::fill(Generator& generator)
{
	std::vector<Type> values(data.size());
	for (auto& value : values)
	{
		value = static_cast<Type>(generator.generate());
	}
	data.swap(values);
}

template<typename Type>
std::string Matrix<Type>::toString() const
{
	if (empty())
	{
		return "{}";
	}
	std::string result;
	for (std::size_t row = 0; row < rows; ++row)
	{
		if (row != 0)
		{
			result += '\n';
		}
		result += "{ ";
		for (std::size_t column = 0; column < columns; ++column)
		{
			if (column != 0)
			{
				result += ", ";
			}
			result += std::to_string(at(row, column));
		}
		result += " }";
	}
	return result;
}

template<typename Type>
void Matrix<Type>::insertColumn(std::size_t index, const std::vector<Type>& values)
{
	if (index > columns)
	{
		throw std::out_of_range("Неверный индекс столбца");
	}
	if (values.size() != rows)
	{
		throw std::invalid_argument("Высота столбца не совпадает с числом строк");
	}
	if (columns == std::numeric_limits<std::size_t>::max())
	{
		throw std::length_error("Слишком много столбцов");
	}
	Matrix result(rows, columns + 1);
	for (std::size_t row = 0; row < rows; ++row)
	{
		for (std::size_t column = 0; column < result.columns; ++column)
		{
			result.at(row, column) = column == index ? values[row]
				: at(row, column < index ? column : column - 1);
		}
	}
	*this = std::move(result);
}

template<typename Type>
Matrix<Type>& Matrix<Type>::operator << (const std::vector<Type>& row)
{
	const std::size_t width = rows == 0 && columns == 0 ? row.size() : columns;
	if (row.size() != width)
	{
		throw std::invalid_argument("Длина строки не совпадает с числом столбцов");
	}
	if (rows == std::numeric_limits<std::size_t>::max())
	{
		throw std::length_error("Слишком много строк");
	}
	data.insert(data.end(), row.begin(), row.end());
	columns = width;
	++rows;
	return *this;
}

template<typename Type>
Matrix<Type>& Matrix<Type>::operator >> (std::vector<Type>& row)
{
	if (rows == 0)
	{
		throw std::out_of_range("В матрице нет строк");
	}
	row.assign(data.begin(), data.begin() + columns);
	data.erase(data.begin(), data.begin() + columns);
	--rows;
	return *this;
}
#endif
