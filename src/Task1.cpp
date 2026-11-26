#include "Task1.h"

Task1::Task1(Matrix<int>& matrix, const Generator& generator, const FillMode mode)
	: Exercise(matrix, generator, mode)
{
}

void Task1::solve()
{
	for (std::size_t row = 0; row < matrix.getRows(); ++row)
	{
		for (std::size_t column = 0; column < matrix.getColumns(); ++column)
		{
			if (matrix[row][column] % 2 != 0)
			{
				matrix[row][column] = 0;
			}
		}
	}
}
