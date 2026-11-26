#ifndef MATRIX_ROW_IMPLEMENTATION
#define MATRIX_ROW_IMPLEMENTATION

#include "MatrixRow.h"
#include <stdexcept>

template<typename Type>
MatrixRow<Type>::MatrixRow(Type* data, const std::size_t columns) requires (!std::is_const_v<Type>)
	: data(data), columns(columns)
{
}

template<typename Type>
MatrixRow<Type>::MatrixRow(const Type* data, const std::size_t columns) requires std::is_const_v<Type>
	: data(data), columns(columns)
{
}

template<typename Type>
MatrixRow<Type>::~MatrixRow() = default;

template<typename Type>
Type& MatrixRow<Type>::operator [] (const std::size_t column) const
{
	if (column >= columns)
	{
		throw std::out_of_range("Выход за границы строки матрицы");
	}
	return data[column];
}

#endif
