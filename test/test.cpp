#include <gtest/gtest.h>
#include "ConstantGenerator.h"
#include "IStreamGenerator.h"
#include "RandomGenerator.h"
#include "Task1.h"
#include "Task2.h"
#include "ZeroGenerator.h"

#include <limits>
#include <sstream>
#include <stdexcept>
#include <type_traits>
#include <utility>

/**
* @brief Проверяет размеры и значения элементов матрицы
* @param actual - полученная матрица
* @param expected - ожидаемая матрица
*/
void expectMatrix(const Matrix<int>& actual, const Matrix<int>& expected)
{
	ASSERT_EQ(actual.getRows(), expected.getRows());
	ASSERT_EQ(actual.getColumns(), expected.getColumns());
	for (std::size_t row = 0; row < expected.getRows(); ++row)
	{
		for (std::size_t column = 0; column < expected.getColumns(); ++column)
		{
			EXPECT_EQ(actual.at(row, column), expected.at(row, column))
				<< "row=" << row << ", column=" << column;
		}
	}
}

TEST(Matrix, Constructors)
{
	Matrix<int> empty;
	EXPECT_TRUE(empty.empty());
	EXPECT_EQ(empty.getRows(), 0);
	EXPECT_EQ(empty.getColumns(), 0);
	Matrix<int> filled(2, 3, 7);
	expectMatrix(filled, {{7, 7, 7}, {7, 7, 7}});
	Matrix<int> zeros(1, 2);
	expectMatrix(zeros, {{0, 0}});
	EXPECT_THROW((Matrix<int>{{1}, {2, 3}}), std::invalid_argument);
	EXPECT_THROW((Matrix<int>(std::numeric_limits<std::size_t>::max(), 2)), std::length_error);
}

TEST(Matrix, CopyAndMoveConstructors)
{
	Matrix<int> source{{1, 2}, {3, 4}};
	Matrix<int> copy(source);
	copy[0][0] = 99;
	EXPECT_EQ(source[0][0], 1);
	Matrix<int> moved(std::move(copy));
	expectMatrix(moved, {{99, 2}, {3, 4}});
	EXPECT_TRUE(copy.empty());
	EXPECT_EQ(copy.getRows(), 0);
	EXPECT_EQ(copy.getColumns(), 0);
	copy = Matrix<int>{{5}};
	expectMatrix(copy, {{5}});
}

TEST(Matrix, AssignmentOperators)
{
	Matrix<int> source{{1, 2}, {3, 4}};
	Matrix<int> copy;
	copy = source;
	copy[0][0] = 9;
	EXPECT_EQ(source[0][0], 1);
	copy = copy;
	expectMatrix(copy, {{9, 2}, {3, 4}});
	Matrix<int> moved;
	moved = std::move(copy);
	expectMatrix(moved, {{9, 2}, {3, 4}});
	EXPECT_EQ(copy.getRows(), 0);
	EXPECT_EQ(copy.getColumns(), 0);
	EXPECT_TRUE(copy.empty());
	moved = std::move(moved);
	expectMatrix(moved, {{9, 2}, {3, 4}});
}

TEST(Matrix, IndexOperators)
{
	Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
	matrix[2][1] = -8;
	matrix.at(0, 2) = 7;
	const Matrix<int>& constant = matrix;
	static_assert(std::is_same_v<decltype(matrix[0][0]), int&>);
	static_assert(std::is_same_v<decltype(constant[0][0]), const int&>);
	EXPECT_EQ(constant[2][1], -8);
	EXPECT_EQ(constant[3][2], 12);
	EXPECT_EQ(matrix.at(2, 1), -8);
	EXPECT_EQ(constant.at(0, 2), 7);
	EXPECT_THROW(matrix[4], std::out_of_range);
	EXPECT_THROW(constant[4], std::out_of_range);
	EXPECT_THROW(matrix[0][3], std::out_of_range);
	EXPECT_THROW(constant[3][3], std::out_of_range);
	EXPECT_THROW(matrix.at(4, 0), std::out_of_range);
	EXPECT_THROW(constant.at(0, 3), std::out_of_range);
}

TEST(Matrix, ShiftOperators)
{
	const Matrix<int> matrix{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
	expectMatrix(matrix << 1, {{2, 3, 4, 1}, {6, 7, 8, 5}, {10, 11, 12, 9}});
	expectMatrix(matrix >> 1, {{4, 1, 2, 3}, {8, 5, 6, 7}, {12, 9, 10, 11}});
	expectMatrix(matrix, {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}});
}

TEST(Matrix, ShiftCounts)
{
	const Matrix<int> matrix{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
	expectMatrix(matrix << 0, matrix);
	expectMatrix(matrix >> 0, matrix);
	expectMatrix(matrix << 4, matrix);
	expectMatrix(matrix >> 4, matrix);
	expectMatrix(matrix << 6, {{3, 4, 1, 2}, {7, 8, 5, 6}, {11, 12, 9, 10}});
	expectMatrix(matrix >> 7, {{2, 3, 4, 1}, {6, 7, 8, 5}, {10, 11, 12, 9}});
}

TEST(Matrix, ShiftMaximumCount)
{
	const Matrix<int> matrix{{1, 2, 3, 4}};
	const Matrix<int> leftResults[] = {
		{{1, 2, 3, 4}}, {{2, 3, 4, 1}}, {{3, 4, 1, 2}}, {{4, 1, 2, 3}}
	};
	const Matrix<int> rightResults[] = {
		{{1, 2, 3, 4}}, {{4, 1, 2, 3}}, {{3, 4, 1, 2}}, {{2, 3, 4, 1}}
	};
	const std::size_t maxShift = std::numeric_limits<std::size_t>::max();
	expectMatrix(matrix << maxShift, leftResults[maxShift % 4]);
	expectMatrix(matrix >> maxShift, rightResults[maxShift % 4]);
}

TEST(Matrix, ShiftEmptyAndSingleColumn)
{
	const std::size_t maxShift = std::numeric_limits<std::size_t>::max();
	for (const auto& matrix : {Matrix<int>(), Matrix<int>(0, 4), Matrix<int>(3, 0)})
	{
		expectMatrix(matrix << 0, matrix);
		expectMatrix(matrix >> 0, matrix);
		expectMatrix(matrix << maxShift, matrix);
		expectMatrix(matrix >> maxShift, matrix);
	}
	const Matrix<int> column{{-5}, {0}, {7}};
	expectMatrix(column << 1, {{-5}, {0}, {7}});
	expectMatrix(column >> 1, {{-5}, {0}, {7}});
	expectMatrix(column << maxShift, {{-5}, {0}, {7}});
	expectMatrix(column >> maxShift, {{-5}, {0}, {7}});
}

TEST(Matrix, ShiftRoundTripAndIndependentResult)
{
	const Matrix<int> matrix{{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}};
	const std::size_t maxShift = std::numeric_limits<std::size_t>::max();
	expectMatrix((matrix << 7) >> 7, matrix);
	expectMatrix((matrix >> maxShift) << maxShift, matrix);
	Matrix<int> zeroShift = matrix << 0;
	Matrix<int> rightShift = matrix >> 1;
	zeroShift[0][0] = 99;
	rightShift[2][3] = -1;
	expectMatrix(matrix, {{1, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}});
	expectMatrix(zeroShift, {{99, 2, 3, 4}, {5, 6, 7, 8}, {9, 10, 11, 12}});
	expectMatrix(rightShift, {{4, 1, 2, 3}, {8, 5, 6, 7}, {12, 9, 10, -1}});
}

TEST(Matrix, ShiftDoubleValues)
{
	const Matrix<double> matrix{{1.25, -2.5, 3.75}, {4.5, 0.0, -6.25}};
	const Matrix<double> left = matrix << 1;
	const Matrix<double> right = matrix >> 1;
	const Matrix<double> expectedLeft{{-2.5, 3.75, 1.25}, {0.0, -6.25, 4.5}};
	const Matrix<double> expectedRight{{3.75, 1.25, -2.5}, {-6.25, 4.5, 0.0}};
	ASSERT_EQ(left.getRows(), 2);
	ASSERT_EQ(left.getColumns(), 3);
	ASSERT_EQ(right.getRows(), 2);
	ASSERT_EQ(right.getColumns(), 3);
	for (std::size_t row = 0; row < 2; ++row)
	{
		for (std::size_t column = 0; column < 3; ++column)
		{
			EXPECT_DOUBLE_EQ(left[row][column], expectedLeft[row][column]);
			EXPECT_DOUBLE_EQ(right[row][column], expectedRight[row][column]);
		}
	}
}

TEST(Matrix, InsertColumn)
{
	Matrix<int> matrix{{1, 2}, {3, 4}};
	matrix.insertColumn(1, Matrix<int>{{8}, {9}});
	expectMatrix(matrix, {{1, 8, 2}, {3, 9, 4}});
	matrix.insertColumn(0, Matrix<int>{{5}, {6}});
	matrix.insertColumn(4, Matrix<int>{{7}, {8}});
	expectMatrix(matrix, {{5, 1, 8, 2, 7}, {6, 3, 9, 4, 8}});
	EXPECT_THROW(matrix.insertColumn(6, Matrix<int>{{1}, {2}}), std::out_of_range);
	EXPECT_THROW(matrix.insertColumn(0, Matrix<int>{{1}}), std::invalid_argument);
}

TEST(Matrix, InsertColumnWrongWidth)
{
	Matrix<int> matrix{{1, 2}, {3, 4}, {5, 6}};
	EXPECT_THROW(matrix.insertColumn(0, Matrix<int>(3, 2)), std::invalid_argument);
	EXPECT_THROW(matrix.insertColumn(0, Matrix<int>(3, 0)), std::invalid_argument);
	expectMatrix(matrix, {{1, 2}, {3, 4}, {5, 6}});
}

TEST(Matrix, ToStringAndDoubleType)
{
	EXPECT_EQ(Matrix<int>().toString(), "{}");
	Matrix<int> matrix{{1, -2}, {0, 4}};
	EXPECT_EQ(matrix.toString(), "|  1 -2 |\n|  0  4 |");
	Matrix<double> real{{1.5, 2.25}};
	EXPECT_DOUBLE_EQ(real[0][1], 2.25);
	EXPECT_EQ(real.toString(), "| 1.500000 2.250000 |");
}

TEST(Matrix, ToStringAlignsAllRows)
{
	Matrix<int> matrix{{1, -20, 300}, {40, 5, -6}, {-1000, 0, 7}};
	EXPECT_EQ(matrix.toString(),
		"|     1   -20   300 |\n"
		"|    40     5    -6 |\n"
		"| -1000     0     7 |");
}

TEST(Matrix, ZeroDimensions)
{
	Matrix<int> noRows(0, 3);
	Matrix<int> noColumns(2, 0);
	EXPECT_TRUE(noRows.empty());
	EXPECT_TRUE(noColumns.empty());
	EXPECT_THROW(noColumns.at(0, 0), std::out_of_range);
	EXPECT_THROW(noColumns[0][0], std::out_of_range);
	EXPECT_THROW(noRows[0], std::out_of_range);
	EXPECT_EQ(noColumns.toString(), "{}");
	EXPECT_EQ(noRows.toString(), "{}");
	noColumns.insertColumn(0, Matrix<int>{{1}, {2}});
	expectMatrix(noColumns, {{1}, {2}});
	EXPECT_EQ(noRows.getRows(), 0);
	EXPECT_EQ(noRows.getColumns(), 3);
	Matrix<int> emptyColumn(0, 1);
	emptyColumn.insertColumn(1, Matrix<int>(0, 1));
	EXPECT_EQ(emptyColumn.getRows(), 0);
	EXPECT_EQ(emptyColumn.getColumns(), 2);
}

TEST(Generators, ConstantAndZero)
{
	Matrix<int> matrix(2, 2);
	ConstantGenerator constant(-7);
	matrix.fill(constant);
	expectMatrix(matrix, {{-7, -7}, {-7, -7}});
	ZeroGenerator zero;
	matrix.fill(zero);
	expectMatrix(matrix, {{0, 0}, {0, 0}});
}

TEST(Generators, RandomBoundsAndSeed)
{
	RandomGenerator first(-3, 5, 42);
	RandomGenerator second(-3, 5, 42);
	for (int i = 0; i < 1000; ++i)
	{
		const int value = first.generate();
		EXPECT_GE(value, -3);
		EXPECT_LE(value, 5);
		EXPECT_EQ(value, second.generate());
	}
	RandomGenerator fixed(7, 7);
	EXPECT_EQ(fixed.generate(), 7);
	EXPECT_THROW((RandomGenerator(3, -3)), std::invalid_argument);
}

TEST(Generators, KeyboardInput)
{
	std::istringstream input("+1 -2 0 4");
	IStreamGenerator generator(input);
	Matrix<int> matrix(2, 2);
	matrix.fill(generator);
	expectMatrix(matrix, {{1, -2}, {0, 4}});
	EXPECT_THROW(generator.generate(), std::runtime_error);
}

TEST(Generators, FailedInputDoesNotChangeMatrix)
{
	for (const auto& text : {"1 abc", "1", "99999999999999999999999999", "1 2.5", "1 2x", "1 +", "1 +-2"})
	{
		std::istringstream input(text);
		IStreamGenerator generator(input);
		Matrix<int> matrix{{7, 8}};
		EXPECT_THROW(matrix.fill(generator), std::runtime_error);
		expectMatrix(matrix, {{7, 8}});
	}
}

TEST(Exercise, PolymorphicFillAndSolve)
{
	Matrix<int> matrix(2, 2);
	ConstantGenerator generator(3);
	Task1 task(matrix, generator);
	Exercise& exercise = task;
	exercise.fill();
	expectMatrix(matrix, {{3, 3}, {3, 3}});
	exercise.solve();
	expectMatrix(matrix, {{0, 0}, {0, 0}});
}

TEST(Task1, ReplacesAllPositiveAndNegativeOddElements)
{
	Matrix<int> matrix{{1, 3, -5, 2}, {-1, 0, -4, 7}};
	ZeroGenerator generator;
	Task1(matrix, generator).solve();
	expectMatrix(matrix, {{0, 0, 0, 2}, {0, 0, -4, 0}});
}

TEST(Task1, IntegerLimits)
{
	const int min = std::numeric_limits<int>::min();
	const int max = std::numeric_limits<int>::max();
	Matrix<int> matrix{{min, max, min + 1}};
	ZeroGenerator generator;
	Task1(matrix, generator).solve();
	expectMatrix(matrix, {{min, 0, 0}});
}

TEST(Task1, NoOddElements)
{
	Matrix<int> matrix{{0, 2}, {-4, 6}};
	ZeroGenerator generator;
	Task1(matrix, generator).solve();
	expectMatrix(matrix, {{0, 2}, {-4, 6}});
}

TEST(Task2, InsertsAfterAllMatchingOriginalColumns)
{
	Matrix<int> matrix{{5, 0, 7, 0}, {8, 2, 0, 0}};
	ZeroGenerator generator;
	Task2(matrix, generator).solve();
	expectMatrix(matrix, {{5, 0, 5, 7, 5, 0, 5}, {8, 2, 8, 0, 8, 0, 8}});
}

TEST(Task2, FirstColumnContainsZero)
{
	Matrix<int> matrix{{0, 4}, {3, 0}};
	ZeroGenerator generator;
	Task2(matrix, generator).solve();
	// Вставленные столбцы повторно не обрабатываются.
	expectMatrix(matrix, {{0, 0, 4, 0}, {3, 3, 0, 3}});
}

TEST(Task2, NoZeros)
{
	Matrix<int> matrix{{1, 2}, {3, 4}};
	ZeroGenerator generator;
	Task2(matrix, generator).solve();
	expectMatrix(matrix, {{1, 2}, {3, 4}});
}

TEST(Task2, AllZeros)
{
	Matrix<int> matrix(2, 3);
	ZeroGenerator generator;
	Task2(matrix, generator).solve();
	expectMatrix(matrix, {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}});
}

TEST(Task2, SingleRowAndSingleColumn)
{
	ZeroGenerator generator;
	Matrix<int> row{{2, 0, 3}};
	Task2(row, generator).solve();
	expectMatrix(row, {{2, 0, 2, 3}});
	Matrix<int> column{{0}, {5}};
	Task2(column, generator).solve();
	expectMatrix(column, {{0, 0}, {5, 5}});
}

TEST(Tasks, EmptyMatrices)
{
	ZeroGenerator generator;
	for (auto matrix : {Matrix<int>(), Matrix<int>(0, 3), Matrix<int>(2, 0)})
	{
		const Matrix<int> original(matrix);
		Task1(matrix, generator).solve();
		Task2(matrix, generator).solve();
		expectMatrix(matrix, original);
	}
}

TEST(Tasks, IndependentResults)
{
	Matrix<int> first{{1, 2}, {3, 0}};
	Matrix<int> second(first);
	ZeroGenerator generator;
	Task1(first, generator).solve();
	Task2(second, generator).solve();
	expectMatrix(first, {{0, 2}, {0, 0}});
	expectMatrix(second, {{1, 2, 1}, {3, 0, 3}});
}

TEST(Tasks, ThreeRowsInputAndBothTasks)
{
	std::istringstream input("5 0 7 2 -3 4 0 -6 8 9 10 0");
	IStreamGenerator generator(input);
	Matrix<int> first(3, 4);
	Task1 task1(first, generator);
	task1.fill();
	Matrix<int> second(first);
	Task2 task2(second, generator);
	task1.solve();
	task2.solve();
	expectMatrix(first, {{0, 0, 0, 2}, {0, 4, 0, -6}, {8, 0, 10, 0}});
	expectMatrix(second, {
		{5, 0, 5, 7, 5, 2, 5},
		{-3, 4, -3, 0, -3, -6, -3},
		{8, 9, 8, 10, 8, 0, 8}
	});
}
