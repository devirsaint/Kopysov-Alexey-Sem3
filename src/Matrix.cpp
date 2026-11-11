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
	data.resize(rows);
	for (std::size_t row = 0; row < rows; ++row)
	{
		data[row].assign(columns, value);
	}
}

template<typename Type>
Matrix<Type>::Matrix(std::initializer_list<std::initializer_list<Type>> values)
	: Matrix(values.size(), values.size() == 0 ? 0 : values.begin()->size())
{
	std::size_t rowIndex = 0;
	for (const auto& row : values)
	{
		if (row.size() != columns)
		{
			throw std::invalid_argument("Строки матрицы должны иметь одинаковую длину");
		}
		std::size_t columnIndex = 0;
		for (const auto& value : row)
		{
			data[rowIndex][columnIndex++] = value;
		}
		++rowIndex;
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
	return rows == 0 || columns == 0;
}

template<typename Type>
Type& Matrix<Type>::at(std::size_t row, std::size_t column)
{
	if (row >= rows || column >= columns)
	{
		throw std::out_of_range("Выход за границы матрицы");
	}
	return data[row][column];
}

template<typename Type>
const Type& Matrix<Type>::at(std::size_t row, std::size_t column) const
{
	if (row >= rows || column >= columns)
	{
		throw std::out_of_range("Выход за границы матрицы");
	}
	return data[row][column];
}

template<typename Type>
MatrixRow<Type> Matrix<Type>::operator [] (std::size_t row)
{
	if (row >= rows)
	{
		throw std::out_of_range("Выход за границы матрицы");
	}
	return MatrixRow<Type>(data[row].data(), columns);
}

template<typename Type>
MatrixRow<const Type> Matrix<Type>::operator [] (std::size_t row) const
{
	if (row >= rows)
	{
		throw std::out_of_range("Выход за границы матрицы");
	}
	return MatrixRow<const Type>(data[row].data(), columns);
}

template<typename Type>
void Matrix<Type>::fill(Generator& generator)
{
	Matrix result(rows, columns);
	for (std::size_t row = 0; row < rows; ++row)
	{
		for (std::size_t column = 0; column < columns; ++column)
		{
			result[row][column] = static_cast<Type>(generator.generate());
		}
	}
	*this = std::move(result);
}

template<typename Type>
std::string Matrix<Type>::toString() const
{
	if (empty())
	{
		return "{}";
	}
	std::size_t width = 0;
	for (std::size_t row = 0; row < rows; ++row)
	{
		for (std::size_t column = 0; column < columns; ++column)
		{
			const std::size_t length = std::to_string(data[row][column]).size();
			if (length > width)
			{
				width = length;
			}
		}
	}
	std::string result;
	for (std::size_t row = 0; row < rows; ++row)
	{
		if (row != 0)
		{
			result += '\n';
		}
		result += "| ";
		for (std::size_t column = 0; column < columns; ++column)
		{
			if (column != 0)
			{
				result += ' ';
			}
			const std::string number = std::to_string(data[row][column]);
			result += std::string(width - number.size(), ' ') + number;
		}
		result += " |";
	}
	return result;
}

template<typename Type>
void Matrix<Type>::insertColumn(std::size_t index, const Matrix& values)
{
	if (index > columns)
	{
		throw std::out_of_range("Неверный индекс столбца");
	}
	if (values.getRows() != rows || values.getColumns() != 1)
	{
		throw std::invalid_argument("Нужна матрица из одного столбца с таким же числом строк");
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
			result.at(row, column) = column == index ? values[row][0]
				: at(row, column < index ? column : column - 1);
		}
	}
	*this = std::move(result);
}

template<typename Type>
Matrix<Type> Matrix<Type>::operator << (const std::size_t shift) const
{
	if (empty())
	{
		return *this;
	}
	const std::size_t offset = shift % columns;
	Matrix result(rows, columns);
	for (std::size_t row = 0; row < rows; ++row)
	{
		std::size_t sourceColumn = offset;
		for (std::size_t column = 0; column < columns; ++column)
		{
			result[row][column] = data[row][sourceColumn];
			++sourceColumn;
			if (sourceColumn == columns)
			{
				sourceColumn = 0;
			}
		}
	}
	return result;
}

template<typename Type>
Matrix<Type> Matrix<Type>::operator >> (const std::size_t shift) const
{
	if (empty())
	{
		return *this;
	}
	return *this << (columns - shift % columns);
}
#endif
