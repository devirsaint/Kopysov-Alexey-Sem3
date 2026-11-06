#pragma once

/**
* @brief Абстрактный класс генератора целых чисел
*/
class Generator
{
public:
	/**
	* @brief Чисто виртуальный деструктор класса
	*/
	virtual ~Generator() = 0;

	/**
	* @brief Получает очередное значение генератора
	* @return Число типа int
	*/
	virtual int generate() = 0;
};
