#pragma once

/**
* @brief Абстрактный класс генератора целых чисел
*/
class Generator
{
public:
	/**
	* @brief Виртуальный деструктор класса
	*/
	virtual ~Generator();

	/**
	* @brief Получает очередное значение генератора
	* @return Число типа int
	*/
	virtual int generate() const = 0;
};
