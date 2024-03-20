#include <iostream>
#include "common_includes.h"
#include "table.h"
#include "io.h"
#include "scanner.h"

int main()
{
   // ¬ариант 5:	
   //    ѕодмножество €зыка —++ включает:
   //    Х данные типа int;
   //    Х инструкции описани€ переменных;
   //    Х операторы присваивани€, switch  любой вложенности и в любой последовательности;
   //    Х операции + , Ц, *, = =, != , < .

   //demo();
   std::ofstream errorStream("..\\error-messages.txt");
   try
   {
      Tables tables = io::readTables(
         "..\\input\\keywords.txt",
         "..\\input\\operators.txt",
         "..\\input\\letters.txt",
         "..\\input\\digits.txt",
         "..\\input\\operatorCharacters.txt",
         "..\\input\\brackets.txt",
         "..\\input\\separators.txt",
         "..\\input\\whitespaces.txt",
         "..\\input\\identifiers.txt",
         "..\\input\\literals.txt"
      );

      scanner::Scanner scanner(tables);
      std::vector<Token> tokens = scanner.scan("..\\test-code.txt", errorStream);
      io::writeTokens("..\\tokens.txt", tokens);

      io::writeAll(tables,
         "..\\output\\keywords.txt",
         "..\\output\\operators.txt",
         "..\\output\\letters.txt",
         "..\\output\\digits.txt",
         "..\\output\\operatorCharacters.txt",
         "..\\output\\brackets.txt",
         "..\\output\\separators.txt",
         "..\\input\\whitespaces.txt",
         "..\\output\\identifiers.txt",
         "..\\output\\literals.txt");
   }
   catch (std::exception const& e) {
      errorStream << e.what() << std::endl;
   }
}
