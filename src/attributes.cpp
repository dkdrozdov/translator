#include "attributes.h"

// Attributes

Attributes::Attributes(SymbolType _symbolType, DataType _dataType, std::string _name, int& _value) :
   symbolType(_symbolType), dataType(_dataType), name(_name), value(_value), memory(sizeof(value)), address(&value) {};

Attributes Attributes::IntVariableAttributes(std::string name, int value) {
   return Attributes(IDENTIFIER, INT, name, value);
}
;

Attributes Attributes::IntLiteralAttributes(int value) {
   return Attributes(LITERAL, INT, "%d"s + std::to_string(value), value);
}


void Attributes::print() const {
   std::cout << "Attributes - symbol type: " << symbolType << "; dataType: " << dataType << "; name: " << name << "; value: " << value << "; memory: " << memory << "; address: " << address << ". ";
}
