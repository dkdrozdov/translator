#pragma once

#include "common_includes.h"
#include "table.h"

/*
   Описывает типы токенов.
   Каждый тип соответствует одной таблице символов.
*/
enum TokenType {
   KEYWORD_TOKEN_TYPE,     // Соответствует таблице ключевых слов.
   OPERATOR_TOKEN_TYPE,    // Соответствует таблице операторов.
   SEPARATOR_TOKEN_TYPE,   // Соответствует таблице разделителей.
   IDENTIFIER_TOKEN_TYPE,  // Соответствует таблице идентификаторов.
   LITERAL_TOKEN_TYPE,     // Соответствует таблице констант.
   BRACKET_TOKEN_TYPE,     // Соответствует таблице скобок.
};

/*
   Описывает токен символа.
   Состоит из номера таблицы и идентификатора, указывающих на местоположение символа.
   Содержит вспомогательные поля для упрощения тестирования.
*/
struct Token {
private:
   TokenType type;

   Tables* tables; // Ссылка на таблицы (необходимая мера для нахождения индекса, поскольку таблицы имён нет).
   int address;   // Индекс в таблице (только для статических таблиц)

public:

   TokenType getType() const;


   /*
      Возвращает истину, если ссылается на динамическую таблицу.
   */
   bool isInMutableTable() const;

   /*
      Вычисляемое поле, поскольку индексы могут рехешироваться в процессе токенизации (для динамических таблиц)
   */
   int getAddress();;

   // Вспомогательные поля:
   int line;         // Номер строки в исходном коде.
   int column;       // Номер столбца в исходном коде.
   std::string name; // Имя символа.

   Token(TokenType _type, int _address, int _line, int _column, std::string _name, Tables* _tables) :
      type(_type), address(_address), line(_line), column(_column), name(_name), tables(_tables) {}
};