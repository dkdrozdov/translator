#include "table.h"

// TableEntry

TableEntry::TableEntry(int index, Attributes _attributes) : tableIndex(index), attributes(_attributes) {}

void TableEntry::print() {
   std::cout << "Table Entry - index: " << tableIndex << ". ";
   attributes.print();
}

Attributes TableEntry::getAttributes() const {
   return attributes;
}

// MutableTable

MutableTable::MutableTable() {
   table = std::vector<TableEntry*>(initial_capacity, nullptr);
   count = 0;
   capacity = initial_capacity;
}

void MutableTable::doubleCapacity() {
   std::vector<TableEntry*> oldTable = table;
   table = std::vector<TableEntry*>(capacity * 2, nullptr);
   count = 0;
   capacity = capacity * 2;

   for (auto e : oldTable) {
      if (e != nullptr)
         add(e->attributes);
   }
}

MutableTable::~MutableTable() {
   for (auto e : table) {
      delete e;
   }
}

TableEntry* MutableTable::find(std::string key) {
   int i = 0;
   int target = hash(key);
   int id = target;
   
   while (!(table[id] != nullptr && table[id]->attributes.name == key) && i <= capacity)
   {
      i++;
      id = (target + i) % capacity;
   }

   if (table[id]->attributes.name == key) {
      return table[id];
   }

   return nullptr;
}

TableEntry* MutableTable::at(int id) {
   return table[id];
};

int MutableTable::hash(std::string key) const {
   int i = 0;

   for (int j = 0; j < key.size(); j++)
      i += key[j] * (j + 1);

   return i % capacity;
};


void MutableTable::add(Attributes attributes) {
   std::string key = attributes.name;
   int identifier = hash(key);

   TableEntry* entry = new TableEntry(identifier, attributes);

   if (table[identifier] == nullptr)   // Место свободно
   {
      table[identifier] = entry;
      count++;
   }
   else {   // Место занято
      int i = 0;
      int id = identifier;

      // Попытка найти подходящее место
      while ((table[id] != nullptr             // Свободное место
         && table[id]->attributes.name != key  // Запись с таким ключом уже имеется
         && i <= capacity))                                             // Места нет и таблица заполнена
      {
         i++;
         id = (identifier + i) % capacity;
      }

      if (table[id] == nullptr) { // Найдено свободное место
         table[id] = entry;
         count++;
      }
      else if (table[id]->attributes.name == key) { // Найдена запись с таким же ключом
         delete table[id];
         table[id] = entry;
      }
      else if (i > capacity) { // В таблице больше нет места
         doubleCapacity();
         add(attributes);
      }
   }

};

void MutableTable::print() {
   std::cout << "Mutable Table - capacity: " << capacity << "; count: " << count << ". " << std::endl;
   for (int i = 0; i < capacity; i++) {
      if (table[i] != nullptr)
         table[i]->print();
      else
         std::cout << "No Entry - index: " << i << ". ";

      std::cout << std::endl;
   }
   std::cout << std::endl;
};

// StaticTable

StaticTable::StaticTable(std::vector<std::string> const& _table) : table(_table) {}

int StaticTable::find(std::string name) {
   auto n = table.size();
   for (int i = 0; i < n; i++)
      if (table[i] == name)
         return i;

   throw std::exception("No entry found.");
};

void StaticTable::print() const {
   std::cout << "Static Table - count: " << table.size() << ". " << std::endl;

   for (int i = 0; i < table.size(); i++) {
      std::cout << "List Element - index: " << i << "; name: " << table[i] << ". " << std::endl;
   }
   std::cout << std::endl;
};