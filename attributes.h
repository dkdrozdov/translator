#pragma once

#include "common_includes.h"

enum AttributeType {
   SYMBOL_TYPE,
   VALUE,
   ADDRESS,
   MEMORY,
   DATA_TYPE,
   NAME
};

enum SymbolType
{
   LITERAL,
   IDENTIFIER,
};

enum DataType {
   INT,
};

struct Attribute {
public:
   virtual AttributeType getType() = 0;
};

struct SymbolTypeAttribute : Attribute {
   const SymbolType symbolType;

public:
   SymbolTypeAttribute(SymbolType type);
   AttributeType getType();
};

struct NameAttribute : Attribute {
   const std::string name;

public:
   NameAttribute(std::string _name);

   AttributeType getType();
};

struct ValueAttribute : Attribute {
   int value;

public:
   ValueAttribute(int _value);

   AttributeType getType();
};

struct AddressAttribute : Attribute {
   const int* address;

public:
   AddressAttribute(int* _address);;

   AttributeType getType();
};

struct MemoryAttribute : Attribute {
   const int memory;

public:
   MemoryAttribute(int _memory);;

   AttributeType getType();
};

struct DataTypeAttribute : Attribute {
   const DataType type;

public:
   DataTypeAttribute(DataType _type);

   AttributeType getType();
};

struct Attributes {
public:
   SymbolType symbolType;
   DataType dataType;
   std::string name;
   ValueAttribute value;
   MemoryAttribute memory;
   AddressAttribute address;

   Attributes(SymbolType _symbolType, DataType _dataType, std::string _name, ValueAttribute _value);;

   static Attributes IntVariableAttributes(std::string name, int value);

   static Attributes IntLiteralAttributes(int value);

   void print() const;
};
