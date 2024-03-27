#include "attributes.h"

// Attributes

Attributes::Attributes(SymbolType _symbolType, DataType _dataType, std::string _name, int& _value) :
   symbolType(_symbolType), dataType(_dataType), name(_name), value(_value), memory(sizeof(value)), address(&value) {};

Attributes Attributes::IntVariableAttributes(std::string name, int value) {
   return Attributes(SymbolType::IDENTIFIER, DataType::INT, name, value);
}
;

Attributes Attributes::IntLiteralAttributes(int value) {
   return Attributes(SymbolType::LITERAL, DataType::INT, "%d"s + std::to_string(value), value);
}


void Attributes::print() const {
   std::cout << "Attributes - symbol type: " << (int)symbolType << "; dataType: " << (int)dataType << "; name: " << name << "; value: " << value << "; memory: " << memory << "; address: " << address << ". ";
}
