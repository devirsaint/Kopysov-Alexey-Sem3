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
#include <utility>
#include <vector>

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
	copy[0] = 99;
	EXPECT_EQ(source[0], 1);
	Matrix<int> moved(std::move(copy));
	expectMatrix(moved, {{99, 2}, {3, 4}});
	EXPECT_TRUE(copy.empty());
	EXPECT_EQ(copy.getRows(), 0);
	EXPECT_EQ(copy.getColumns(), 0);
	copy << std::vector<int>{5};
	expectMatrix(copy, {{5}});
}

TEST(Matrix, AssignmentOperators)
{
	Matrix<int> source{{1, 2}, {3, 4}};
	Matrix<int> copy;
	copy = source;
	copy[0] = 9;
	EXPECT_EQ(source[0], 1);
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
	Matrix<int> matrix{{1, 2, 3}, {4, 5, 6}};
	matrix[4] = 8;
	matrix.at(0, 2) = 7;
	const Matrix<int>& constant = matrix;
	EXPECT_EQ(constant[4], 8);
	EXPECT_EQ(constant.at(0, 2), 7);
	EXPECT_THROW(matrix[6], std::out_of_range);
	EXPECT_THROW(constant[6], std::out_of_range);
	EXPECT_THROW(matrix.at(2, 0), std::out_of_range);
	EXPECT_THROW(constant.at(0, 3), std::out_of_range);
}

TEST(Matrix, ShiftOperators)
{
	Matrix<int> matrix;
	matrix << std::vector<int>{1, 2} << std::vector<int>{3, 4};
	expectMatrix(matrix, {{1, 2}, {3, 4}});
	std::vector<int> row;
	matrix >> row;
	EXPECT_EQ(row, (std::vector<int>{1, 2}));
	expectMatrix(matrix, {{3, 4}});
	EXPECT_THROW(matrix << std::vector<int>{9}, std::invalid_argument);
	matrix >> row;
	EXPECT_EQ(row, (std::vector<int>{3, 4}));
	EXPECT_TRUE(matrix.empty());
	EXPECT_EQ(matrix.getColumns(), 2);
	EXPECT_THROW(matrix >> row, std::out_of_range);
	matrix << std::vector<int>{5, 6};
	expectMatrix(matrix, {{5, 6}});
}

TEST(Matrix, InsertColumn)
{
	Matrix<int> matrix{{1, 2}, {3, 4}};
	matrix.insertColumn(1, {8, 9});
	expectMatrix(matrix, {{1, 8, 2}, {3, 9, 4}});
	matrix.insertColumn(0, {5, 6});
	matrix.insertColumn(4, {7, 8});
	expectMatrix(matrix, {{5, 1, 8, 2, 7}, {6, 3, 9, 4, 8}});
	EXPECT_THROW(matrix.insertColumn(6, {1, 2}), std::out_of_range);
	EXPECT_THROW(matrix.insertColumn(0, {1}), std::invalid_argument);
}

TEST(Matrix, ToStringAndDoubleType)
{
	EXPECT_EQ(Matrix<int>().toString(), "{}");
	Matrix<int> matrix{{1, -2}, {0, 4}};
	EXPECT_EQ(matrix.toString(), "{ 1, -2 }\n{ 0, 4 }");
	Matrix<double> real{{1.5, 2.25}};
	EXPECT_DOUBLE_EQ(real[1], 2.25);
	EXPECT_EQ(real.toString(), "{ 1.500000, 2.250000 }");
}

TEST(Matrix, ZeroDimensions)
{
	Matrix<int> noRows(0, 3);
	Matrix<int> noColumns(2, 0);
	EXPECT_TRUE(noRows.empty());
	EXPECT_TRUE(noColumns.empty());
	EXPECT_THROW(noColumns.at(0, 0), std::out_of_range);
	noColumns.insertColumn(0, {1, 2});
	expectMatrix(noColumns, {{1}, {2}});
	noRows << std::vector<int>{1, 2, 3};
	expectMatrix(noRows, {{1, 2, 3}});
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
