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

   // Выход при следующих условиях:
   // 1. найдено совпадающее имя в ненулевой записи
   // 1. цикл пошёл по второму кругу
   while (!(table[id] != nullptr && table[id]->attributes.name == key) && i <= capacity)
   {
      i++;
      id = (target + i) % capacity;
   }

   if (table[id] != nullptr && table[id]->attributes.name == key) {
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


int MutableTable::add(Attributes attributes) {
   std::string key = attributes.name;
   int identifier = hash(key);


   if (table[identifier] == nullptr)   // Место свободно
   {
      TableEntry* entry = new TableEntry(identifier, attributes);
      table[identifier] = entry;
      count++;

      return identifier;
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
         TableEntry* entry = new TableEntry(id, attributes);
         table[id] = entry;
         count++;

         return id;
      }
      else if (table[id]->attributes.name == key) { // Найдена запись с таким же ключом
         delete table[id];

         TableEntry* entry = new TableEntry(id, attributes);
         table[id] = entry;

         return id;
      }
      else if (i > capacity) { // В таблице больше нет места
         doubleCapacity();
         return add(attributes);
      }
   }
   throw std::exception("Couldn't add table entry.");
}
void MutableTable::remove(std::string key)
{
   int id = find(key)->tableIndex;

   delete table[id];

   table[id] = nullptr;
}
;

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
}

bool StaticTable::contains(std::string name)
{
   try {
      find(name);
   }
   catch (std::exception) {
      return false;
   }

   return true;
};

void StaticTable::print() const {
   std::cout << "Static Table - count: " << table.size() << ". " << std::endl;

   for (int i = 0; i < table.size(); i++) {
      std::cout << "List Element - index: " << i << "; name: " << table[i] << ". " << std::endl;
   }
   std::cout << std::endl;
};


// Tables

Tables::Tables(StaticTable _keywords, StaticTable _operators, StaticTable _letters, StaticTable _digits, StaticTable _operatorCharacters, StaticTable _brackets, StaticTable _separators, StaticTable _whitespaces, MutableTable* _identifiers, MutableTable* _literals) :
    keywords(_keywords),
    operators(_operators),
    letters(_letters),
    digits(_digits),
    operatorCharacters(_operatorCharacters),
    brackets(_brackets),
    separators(_separators),
    whitespaces(_whitespaces),
    identifiers(_identifiers),
    literals(_literals) {}

CharacterClass Tables::classify(char c) {
    std::string cs = { c };

    if (letters.contains(cs)) return LETTER_CHARACTER;
    if (digits.contains(cs)) return DIGIT_CHARACTER;
    if (operatorCharacters.contains(cs)) return OPERATOR_CHARACTER_CHARACTER;
    if (brackets.contains(cs)) return BRACKET_CHARACTER;
    if (separators.contains(cs)) return SEPARATOR_CHARACTER;
    if (whitespaces.contains(cs)) return WHITESPACE_CHARACTER;
    if (cs == "=") return EQUAL_SIGN_CHARACTER;
    if (cs == "!") return EXCLAMATION_MARK_CHARACTER;

    return INVALID_CHARACTER;
}
