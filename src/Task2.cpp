#include "Task2.h"
#include <limits>
#include <stdexcept>
#include <utility>

Task2::Task2(Matrix<int>& matrix, Generator& generator)
	: Exercise(matrix, generator)
{
}

void Task2::solve()
{
	if (matrix.empty())
	{
		return;
	}
	const std::size_t columns = matrix.getColumns();
	Matrix<int> containsZero(1, columns, 0);
	std::size_t added = 0;
	for (std::size_t column = 0; column < columns; ++column)
	{
		for (std::size_t row = 0; row < matrix.getRows(); ++row)
		{
			if (matrix[row][column] == 0)
			{
				containsZero[0][column] = 1;
				++added;
				break;
			}
		}
	}
	if (added == 0)
	{
		return;
	}
	if (columns > std::numeric_limits<std::size_t>::max() - added)
	{
		throw std::length_error("Слишком много столбцов в результате");
	}
	Matrix<int> result(matrix.getRows(), columns + added);
	for (std::size_t row = 0; row < matrix.getRows(); ++row)
	{
		std::size_t target = 0;
		for (std::size_t column = 0; column < columns; ++column)
		{
			result[row][target++] = matrix[row][column];
			if (containsZero[0][column] != 0)
			{
				result[row][target++] = matrix[row][0];
			}
		}
	}
	matrix = std::move(result);
}
