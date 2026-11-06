#include "ConstantGenerator.h"
#include "IStreamGenerator.h"
#include "RandomGenerator.h"
#include "Task1.h"
#include "Task2.h"
#include "ZeroGenerator.h"

#include <exception>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#endif

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
		Task2 second(secondMatrix, *generator);
		first.solve();
		second.solve();
		std::cout << "\nЗадание 1. Все нечётные элементы заменены нулями:\n"
			<< original.toString() << '\n';
		std::cout << "\nЗадание 2. После каждого исходного столбца с нулём вставлен первый столбец:\n"
			<< secondMatrix.toString() << '\n';

		Matrix<int> example;
		example = original;
		std::cout << "\nКопирование оператором =, первый элемент через [0]: " << example[0] << '\n';
		example << std::vector<int>(example.getColumns(), 2);
		std::cout << "После добавления строки из двоек оператором <<:\n" << example.toString() << '\n';
		std::vector<int> extracted;
		example >> extracted;
		std::cout << "После извлечения первой строки оператором >>:\n" << example.toString() << '\n';
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
	std::cout << "Способ заполнения:\n1 — случайные числа\n2 — ввод с клавиатуры\n"
		<< "3 — нули\n4 — постоянное значение\nВаш выбор: ";
	switch (getNumber())
	{
	case 1:
	{
		std::cout << "Введите минимум и максимум диапазона: ";
		const int min = getNumber();
		const int max = getNumber();
		return std::make_unique<RandomGenerator>(min, max);
	}
	case 2:
		std::cout << "Введите элементы матрицы построчно:\n";
		return std::make_unique<IStreamGenerator>(std::cin);
	case 3:
		return std::make_unique<ZeroGenerator>();
	case 4:
		std::cout << "Введите постоянное значение: ";
		return std::make_unique<ConstantGenerator>(getNumber());
	default:
		throw std::invalid_argument("Нет такого способа заполнения");
	}
}
