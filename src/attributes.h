#pragma once

#include "common_includes.h"

/*
  Определяет типы символов.
*/
enum SymbolType
{
   LITERAL,    // Тип константы.
   IDENTIFIER, // Тип идентификатора.
};

/*
  Определяет типы данных.
*/
enum DataType {
   INT,        // Тип целочисленных данных.
};

/*
   Определяет изменяемый набор атрибутов записи таблицы символов.
*/
struct Attributes {
public:
   SymbolType symbolType;
   DataType dataType;
   std::string name;       // Имя символа. Для целочисленных констант задаётся как %d и численное значение константы.
   int value;              // Целочисленное значение символа.
   int* address;           // Адрес области памяти, выделенной для хранения значения.
   int memory;             // Размер выделенной области памяти.

   Attributes(SymbolType _symbolType, DataType _dataType, std::string _name, int& _value);

   /*
      Создаёт аттрибуты для идентификатора целочисленной переменной.
   */
   static Attributes IntVariableAttributes(std::string name, int value);

   /*
      Создаёт аттрибуты для идентификатора целочисленной константы.
   */
   static Attributes IntLiteralAttributes(int value);

   /*
      Выводит в консоль атрибуты.
   */
   void print() const;
};
