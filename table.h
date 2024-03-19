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
      Создаёт и добавляет в таблицу новую запись с атрибутами attributes.
   */
   void add(Attributes attributes);

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
};