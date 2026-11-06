#include "Task1.h"

Task1::Task1(Matrix<int>& matrix, Generator& generator)
	: Exercise(matrix, generator)
{
}

void Task1::solve()
{
	for (std::size_t row = 0; row < matrix.getRows(); ++row)
	{
		for (std::size_t column = 0; column < matrix.getColumns(); ++column)
		{
			if (matrix.at(row, column) % 2 != 0)
			{
				matrix.at(row, column) = 0;
			}
		}
	}
}
