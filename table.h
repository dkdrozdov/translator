#pragma once

#include "common_includes.h"
#include "attributes.h"

/*
   ��������� ������ ������� ��������.
*/
struct TableEntry {
   int tableIndex;         // ������ ������ � ������� ��������.
   Attributes attributes;

   TableEntry(int index, Attributes _attributes);

   void print(); // ������� ������ � �������.

   Attributes getAttributes() const;
};

constexpr int initial_capacity = 4; // ����������� ����� ������������ �������.

/*
   ��������� ������������ ������� ��������.
*/
struct MutableTable {
   std::vector<TableEntry*> table;
   int count;     // ���������� �������� ������� � �������.
   int capacity;  // ����� ���������� �������.

   MutableTable();

   ~MutableTable();

   /*
      ���������� ����� ������ �� ����� ������� key.
      ���������� ��������� �� ������.
   */
   TableEntry* find(std::string key);

   /*
      ���������� ��������� �� ������, ������������ �� ������� id.
   */
   TableEntry* at(int id);

   /*
      �������� ��� ������� key � ���������� ��� ������ � �������.
   */
   int hash(std::string key) const;

   /*
      ������ � ��������� � ������� ����� ������ � ���������� attributes.
   */
   void add(Attributes attributes);

   /*
      ������� �� ������� ������ �� ����� ������� key.
   */
   void remove(std::string key);

   /*
      ��������� ����� ������� � ���������� � ������.
   */
   void doubleCapacity();

   /*
      ������� � ������� ������ �������.
   */
   void print();
};

/*
   ��������� ����������� ������� ��������.
*/
struct StaticTable {
   const std::vector<std::string> table;

   StaticTable(std::vector<std::string> const& _table);

   /*
      ������� � ������� ������ �������.
   */
   void print() const;

   /*
      ���������� ����� ������ name � ���������� � ������ � �������.
   */
   int find(std::string name);
};