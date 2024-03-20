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

   std::ofstream errorStream("..\\error-messages.txt");
   try
   {
      /*       „тение вводных таблиц.        */
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

      /*       —оздание сканера и запуск лексического анализа.        */
      scanner::Scanner scanner(tables);      
      std::vector<Token> tokens = scanner.scan("..\\test-code.txt", errorStream);

      /*       ¬ывод таблицы токенов и символов в соответствующие файлы.        */
      io::writeTokens("..\\tokens.txt", tokens);
      io::writeAll(tables,
         "..\\output\\keywords.txt",
         "..\\output\\operators.txt",
         "..\\output\\letters.txt",
         "..\\output\\digits.txt",
         "..\\output\\operatorCharacters.txt",
         "..\\output\\brackets.txt",
         "..\\output\\separators.txt",
         "..\\output\\whitespaces.txt",
         "..\\output\\identifiers.txt",
         "..\\output\\literals.txt");
   }
   /*          ќбработка критических ошибок.           */
   catch (std::exception const& e) {
      errorStream << e.what() << std::endl;
   }
}
