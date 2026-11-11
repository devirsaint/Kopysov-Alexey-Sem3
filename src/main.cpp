#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "ConstantGenerator.h"
#include "IStreamGenerator.h"
#include "RandomGenerator.h"
#include "Task1.h"
#include "Task2.h"
#include "ZeroGenerator.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

/**
* @brief Способы заполнения матрицы
*/
enum class FillMethod
{
	Random = 1,
	Keyboard,
	Zero,
	Constant
};

/**
* @brief Считывает целое число с клавиатуры с проверкой ввода
* @return Считанное число типа int
*/
int getNumber();

/**
* @brief Создаёт генератор по выбору пользователя
* @return Указатель на выбранный генератор
*/
std::unique_ptr<Generator> getGenerator();

/**
* @brief Точка входа в программу, демонстрирует работу с матрицей
* @return 0 если программа выполнена корректно, иначе 1
*/
int main()
{
#ifdef _WIN32
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
#endif
	try
	{
		std::cout << "Вариант 14. Двумерный массив\nВведите количество строк и столбцов: ";
		const int rows = getNumber();
		const int columns = getNumber();
		if (rows <= 0 || columns <= 0)
		{
			throw std::invalid_argument("Размеры матрицы должны быть положительными");
		}
		Matrix<int> original(static_cast<std::size_t>(rows), static_cast<std::size_t>(columns));
		std::unique_ptr<Generator> generator = getGenerator();
		Task1 first(original, *generator);
		first.fill();
		std::cout << "\nИсходная матрица:\n" << original.toString() << '\n';

		// Сохраняем исходную матрицу для второго задания
		Matrix<int> secondMatrix(original);
		Matrix<int> example;
		example = original;
		Task2 second(secondMatrix, *generator);
		first.solve();
		second.solve();
		std::cout << "\nЗадание 1. Все нечётные элементы заменены нулями:\n"
			<< original.toString() << '\n';
		std::cout << "\nЗадание 2. После каждого исходного столбца с нулём вставлен первый столбец:\n"
			<< secondMatrix.toString() << '\n';

		std::cout << "\nКопия исходной матрицы через =, первый элемент через [0][0]: " << example[0][0] << '\n';
		std::cout << "Введите количество позиций сдвига столбцов (0 или больше): ";
		const int count = getNumber();
		if (count < 0)
		{
			throw std::invalid_argument("Количество позиций сдвига не может быть отрицательным");
		}
		const std::size_t shift = static_cast<std::size_t>(count);
		const Matrix<int> left = example << shift;
		const Matrix<int> right = example >> shift;
		std::cout << "\nИсходная матрица после циклического сдвига столбцов влево на " << shift
			<< ":\n" << left.toString() << '\n';
		std::cout << "\nИсходная матрица после циклического сдвига столбцов вправо на " << shift
			<< ":\n" << right.toString() << '\n';
	}
	catch (const std::exception& error)
	{
		std::cerr << "Ошибка: " << error.what() << '\n';
		return 1;
	}
	return 0;
}

int getNumber()
{
	IStreamGenerator input(std::cin);
	return input.generate();
}

std::unique_ptr<Generator> getGenerator()
{
	std::cout << "Способ заполнения:\n"
		<< static_cast<int>(FillMethod::Random) << " — случайные числа\n"
		<< static_cast<int>(FillMethod::Keyboard) << " — ввод с клавиатуры\n"
		<< static_cast<int>(FillMethod::Zero) << " — нули\n"
		<< static_cast<int>(FillMethod::Constant) << " — постоянное значение\nВаш выбор: ";
	const FillMethod method = static_cast<FillMethod>(getNumber());
	switch (method)
	{
	case FillMethod::Random:
	{
		std::cout << "Введите минимум и максимум диапазона: ";
		const int min = getNumber();
		const int max = getNumber();
		return std::make_unique<RandomGenerator>(min, max);
	}
	case FillMethod::Keyboard:
		std::cout << "Введите элементы матрицы построчно:\n";
		return std::make_unique<IStreamGenerator>(std::cin);
	case FillMethod::Zero:
		return std::make_unique<ZeroGenerator>();
	case FillMethod::Constant:
		std::cout << "Введите постоянное значение: ";
		return std::make_unique<ConstantGenerator>(getNumber());
	default:
		throw std::invalid_argument("Нет такого способа заполнения");
	}
}
