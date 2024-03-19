#include <iostream>
#include "common_includes.h"
#include "table.h"
#include "io.h"

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

int main()
{
   // ¬ариант 5:	
   //    ѕодмножество €зыка —++ включает:
   //    Х данные типа int;
   //    Х инструкции описани€ переменных;
   //    Х операторы присваивани€, switch  любой вложенности и в любой последовательности;
   //    Х операции + , Ц, *, = =, != , < .

   //demo();
   try
   {
      StaticTable keywords = io::readStaticTable("..\\input\\keywords.txt");
      StaticTable operators = io::readStaticTable("..\\input\\operators.txt");
      StaticTable separators = io::readStaticTable("..\\input\\separators.txt");
      StaticTable alphabet = io::readStaticTable("..\\input\\alphabet.txt");

      MutableTable identifiers = io::readMutableTable("..\\input\\identifiers.txt");
      MutableTable literals = io::readMutableTable("..\\input\\literals.txt");

      keywords.print();
      operators.print();
      separators.print();
      alphabet.print();

      identifiers.print();
      literals.print();

      identifiers.remove("i");

      io::writeStaticTable("..\\output\\keywords.txt", keywords);
      io::writeStaticTable("..\\output\\operators.txt", operators);
      io::writeStaticTable("..\\output\\separators.txt", separators);
      io::writeStaticTable("..\\output\\alphabet.txt", alphabet);
      io::writeMutableTable("..\\output\\identifiers.txt", identifiers);
      io::writeMutableTable("..\\output\\literals.txt", literals);
   }
   catch (std::exception const& e) {
      std::cerr << e.what() << std::endl;
   }
}
