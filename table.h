#pragma once

#include "common_includes.h"
#include "attributes.h"


struct TableEntry {
   int tableIndex;
   Attributes attributes;

public:
   TableEntry(int index, Attributes _attributes);

   void print();

   Attributes getAttributes() const;
};

constexpr int initial_capacity = 4;

struct MutableTable {
   std::vector<TableEntry*> table;
   int count;
   int capacity;

public:
   MutableTable();

   ~MutableTable();

   TableEntry* find(std::string key);

   TableEntry* at(int id);

   int hash(std::string key) const;

   void add(Attributes attributes);

   void doubleCapacity();

   void print();
};

struct StaticTable {
   const std::vector<std::string> table;

public:
   StaticTable(std::vector<std::string> const& _table);

   void print() const;

   int find(std::string lexeme);
};