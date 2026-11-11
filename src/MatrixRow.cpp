#ifndef MATRIX_ROW_IMPLEMENTATION
#define MATRIX_ROW_IMPLEMENTATION

#include "MatrixRow.h"
#include <stdexcept>

template<typename Type>
MatrixRow<Type>::MatrixRow(Type* data, std::size_t columns)
	: data(data), columns(columns)
{
}

template<typename Type>
Type& MatrixRow<Type>::operator [] (std::size_t column) const
{
	if (column >= columns)
	{
		throw std::out_of_range("Выход за границы строки матрицы");
	}
	return data[column];
}

#endif
