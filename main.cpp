#include <iostream>
#include "common_includes.h"
#include "table.h"

static StaticTable generateKeywords() {
   std::vector<std::string> entries = {
      "switch",
      "int",
   };

   return StaticTable(entries);
}

static StaticTable generateOperators() {
   std::vector<std::string> entries = {
      "+",
      "-",
      "*",
      "==",
      "!=",
      "<",
      ">",
      "=",
   };
   return StaticTable(entries);
}

static StaticTable generateSeparators() {
   std::vector<std::string> entries = {
      " ",
      "\t",
      ";",
      ",",
   };
   return StaticTable(entries);
}

static void demo() {
   StaticTable keywords = generateKeywords();
   StaticTable operators = generateOperators();
   StaticTable separators = generateSeparators();

   MutableTable identifiers = MutableTable();
   MutableTable literals = MutableTable();

   std::cout << "keywords:" << std::endl;
   keywords.print();
   std::cout << "operators:" << std::endl;
   operators.print();
   std::cout << "separators:" << std::endl;
   separators.print();
   std::cout << "identifiers:" << std::endl;
   identifiers.print();
   std::cout << "literals:" << std::endl;
   literals.print();

   // ��������� �������������:
   identifiers.add(Attributes::IntVariableAttributes("i", 3));
   std::cout << "identifiers:" << std::endl;
   identifiers.print();


   // ��������� ���������:
   literals.add(Attributes::IntLiteralAttributes(55));
   literals.add(Attributes::IntLiteralAttributes(63));
   literals.add(Attributes::IntLiteralAttributes(-10));
   std::cout << "literals:" << std::endl;
   literals.print();

   // ����������� �������, ���������� ����������:
   identifiers.add(Attributes::IntVariableAttributes("j", 4));
   identifiers.add(Attributes::IntVariableAttributes("k", 5));
   identifiers.add(Attributes::IntVariableAttributes("l", 6));
   identifiers.add(Attributes::IntVariableAttributes("m", 7));
   identifiers.add(Attributes::IntVariableAttributes("n", 8));
   identifiers.add(Attributes::IntVariableAttributes("o", 9));
   std::cout << "identifiers:" << std::endl;
   identifiers.print();

   // ���� �������:
   std::cout << "\"k\" lexeme search:" << std::endl;
   identifiers.find("k")->print();

   // ���� �������������� �������:
   std::cout << "\n\"qwe\" lexeme search:" << std::endl;
   std::cout << ((identifiers.find("qwe") == nullptr) ? "not found" : "found") << std::endl;

   // ���� �������� � ������ �������� �������:
   std::cout << "\nset \"k\" value to -100:" << std::endl;
   identifiers.find("k")->attributes.value.value = -100;
   identifiers.print();
}

int main()
{
   // ������� 5:	
   //    ������������ ����� �++ ��������:
   //    � ������ ���� int;
   //    � ���������� �������� ����������;
   //    � ��������� ������������, switch  ����� ����������� � � ����� ������������������;
   //    � �������� + , �, *, = =, != , < .

   demo();
}
