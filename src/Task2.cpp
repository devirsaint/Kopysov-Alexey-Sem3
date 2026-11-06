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
	std::vector<bool> containsZero(columns, false);
	std::size_t added = 0;
	for (std::size_t column = 0; column < columns; ++column)
	{
		for (std::size_t row = 0; row < matrix.getRows(); ++row)
		{
			if (matrix.at(row, column) == 0)
			{
				containsZero[column] = true;
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
			result.at(row, target++) = matrix.at(row, column);
			if (containsZero[column])
			{
				result.at(row, target++) = matrix.at(row, 0);
			}
		}
	}
	matrix = std::move(result);
}
