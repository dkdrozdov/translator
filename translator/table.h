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
      ������ � ��������� � ������� ����� ������ � ���������� attributes, ���� � ��� � �������.
      � ������, ���� ����� ������ ��� ����, �������������� �.
      ���������� ������ ������ � �������.
   */
   int add(Attributes attributes);

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

   /*
      ���������� true, ���� ������ name ���������� � �������, ����� false.
   */
   bool contains(std::string name);
};

/*
   ��������� ������ �������� ��������.
*/
enum CharacterClass {
   LETTER_CHARACTER,             // ��������� ������� [A-Za-z].
   DIGIT_CHARACTER,              // ����� [0-9].
   OPERATOR_CHARACTER_CHARACTER, // ����� ���������� +|-|*|<|>.
   EQUAL_SIGN_CHARACTER,         // ���� ��������� =.
   EXCLAMATION_MARK_CHARACTER,   // ��������������� ���� !.
   SEPARATOR_CHARACTER,          // �������������� ������� ,|;|:.
   WHITESPACE_CHARACTER,         // ������ ������� (���������, �������, �������� ��������).
   BRACKET_CHARACTER,            // ������ (|)|{|}.
   INVALID_CHARACTER,            // ������������ ������� (��� ���������).
   COUNT_CHARACTER               // ��������������� ������ ��� �������� ���������� �����.
};

/*
   ��������� ��������� ������ ������.
   ������������ ������ ������� ���, ����������� ������� ��������� ���������.
   �������� ��� ����������� � ������������ �������.
   ��. �����: io::readTables, io::writeAll.
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
      �������������� ������ c �� ������� �������� ��������.
      ���������� ������ �������� CharacterClass.
   */
   CharacterClass classify(char c);
};