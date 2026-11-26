#include "Exercise.h"

Exercise::Exercise(Matrix<int>& matrix, const Generator& generator, const FillMode mode) : matrix(matrix), generator(generator)
{
	if (mode == FillMode::Generate)
	{
		this->matrix.fill(this->generator);
	}
}

Exercise::~Exercise() = default;

void Exercise::fill()
{
	matrix.fill(generator);
}
