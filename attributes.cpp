#include "attributes.h"

// LanguageUnitAttribute

SymbolTypeAttribute::SymbolTypeAttribute(SymbolType type) : symbolType(type) {}

AttributeType SymbolTypeAttribute::getType() {
   return SYMBOL_TYPE;
}


// ValueAttribute

ValueAttribute::ValueAttribute(int _value) : value(_value) {}

AttributeType ValueAttribute::getType() {
   return VALUE;
}

// Attributes

Attributes::Attributes(SymbolType _symbolType, DataType _dataType, std::string _name, ValueAttribute _value) :
   symbolType(_symbolType), dataType(_dataType), name(_name), value(_value), memory(sizeof(value.value)), address((int*)value.value) {};

Attributes Attributes::IntVariableAttributes(std::string name, int value) {
   return Attributes(IDENTIFIER, INT, name, ValueAttribute(value));
}
;

Attributes Attributes::IntLiteralAttributes(int value) {
   return Attributes(LITERAL, INT, "%d" + std::to_string(value), ValueAttribute(value));
}


void Attributes::print() const {
   std::cout << "Attributes - symbol type: " << symbolType << "; dataType: " << dataType << "; name: " << name << "; value: " << value.value << "; memory: " << memory.memory << "; address: " << address.address << ". ";
}

// AddressAttribute

AddressAttribute::AddressAttribute(int* _address) : address(_address) {}

AttributeType AddressAttribute::getType()
{
   return ADDRESS;
}

// MemoryAttribute

MemoryAttribute::MemoryAttribute(int _memory) : memory(_memory) {}

AttributeType MemoryAttribute::getType()
{
   return MEMORY;
}

// DataTypeAttribute

DataTypeAttribute::DataTypeAttribute(DataType _type) : type(_type) {}

AttributeType DataTypeAttribute::getType()
{
   return DATA_TYPE;
}

// NameAttribute

NameAttribute::NameAttribute(std::string _name) : name(_name) {}

AttributeType NameAttribute::getType()
{
   return NAME;
}
