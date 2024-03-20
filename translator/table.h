#pragma once

#include "common_includes.h"
#include "attributes.h"

/*
   Описывает запись таблицы символов.
*/
struct TableEntry {
   int tableIndex;         // Индекс записи в таблице символов.
   Attributes attributes;

   TableEntry(int index, Attributes _attributes);

   void print(); // Выводит запись в консоль.

   Attributes getAttributes() const;
};

constexpr int initial_capacity = 4; // Изначальный объём динамической таблицы.

/*
   Описывает динамическую таблицу символов.
*/
struct MutableTable {
   std::vector<TableEntry*> table;
   int count;     // Количество непустых записей в таблице.
   int capacity;  // Общее количество записей.

   MutableTable();

   ~MutableTable();

   /*
      Производит поиск записи по имени символа key.
      Возвращает указатель на запись.
   */
   TableEntry* find(std::string key);

   /*
      Возвращает указатель на запись, находяющуюся по индексу id.
   */
   TableEntry* at(int id);

   /*
      Хеширует имя символа key и возвращает его индекс в таблице.
   */
   int hash(std::string key) const;

   /*
      Создаёт и добавляет в таблицу новую запись с атрибутами attributes, если её нет в таблице.
      В случае, если такая запись уже есть, перезаписывает её.
      Возвращает индекс записи в таблице.
   */
   int add(Attributes attributes);

   /*
      Удаляет из таблицы запись по имени символа key.
   */
   void remove(std::string key);

   /*
      Удваивает объём таблицы и рехеширует её записи.
   */
   void doubleCapacity();

   /*
      Выводит в консоль записи таблицы.
   */
   void print();
};

/*
   Описывает статическую таблицу символов.
*/
struct StaticTable {
   const std::vector<std::string> table;

   StaticTable(std::vector<std::string> const& _table);

   /*
      Выводит в консоль записи таблицы.
   */
   void print() const;

   /*
      Производит поиск записи name и возвращает её индекс в таблице.
   */
   int find(std::string name);

   /*
      Возвращает true, если запись name существует в таблице, иначе false.
   */
   bool contains(std::string name);
};

/*
   Описывает группы символов перехода.
*/
enum CharacterClass {
   LETTER_CHARACTER,             // Латинский алфавит [A-Za-z].
   DIGIT_CHARACTER,              // Цифры [0-9].
   OPERATOR_CHARACTER_CHARACTER, // Знаки операторов +|-|*|<|>.
   EQUAL_SIGN_CHARACTER,         // Знак равенства =.
   EXCLAMATION_MARK_CHARACTER,   // Восклицательный знак !.
   SEPARATOR_CHARACTER,          // Разделительные символы ,|;|:.
   WHITESPACE_CHARACTER,         // Пустые символы (табуляции, пробелы, строчные переходы).
   BRACKET_CHARACTER,            // Скобки (|)|{|}.
   INVALID_CHARACTER,            // Недопустимые символы (все остальные).
   COUNT_CHARACTER               // Вспомогательная запись для подсчёта количества групп.
};

/*
   Описывает структуру связки таблиц.
   Используется вместо таблицы имён, значительно упрощая структуру программы.
   Содержит все статические и динамические таблицы.
   См. также: io::readTables, io::writeAll.
*/
struct Tables {
   StaticTable keywords;
   StaticTable operators;
   StaticTable letters;
   StaticTable digits;
   StaticTable operatorCharacters;
   StaticTable brackets;
   StaticTable separators;
   StaticTable whitespaces;

   MutableTable* identifiers;
   MutableTable* literals;

   Tables(StaticTable _keywords,
      StaticTable _operators,
      StaticTable _letters,
      StaticTable _digits,
      StaticTable _operatorCharacters,
      StaticTable _brackets,
      StaticTable _separators,
      StaticTable _whitespaces,
      MutableTable* _identifiers,
      MutableTable* _literals);

   /* 
      Классифицирует символ c по группам символов перехода.
      Возвращает группу перехода CharacterClass.
   */
   CharacterClass classify(char c);
};