#include <iostream>
#include "common_includes.h"
#include "table.h"
#include "io.h"
#include "scanner.h"

int main()
{
   // Вариант 5:	
   //    Подмножество языка С++ включает:
   //    • данные типа int;
   //    • инструкции описания переменных;
   //    • операторы присваивания, switch  любой вложенности и в любой последовательности;
   //    • операции + , –, *, = =, != , < .

   std::ofstream errorStream("output\\error-messages.txt");
   /*       Чтение вводных таблиц.        */
   Tables* tables = io::readTables(
      "input\\tables\\keywords.txt",
      "input\\tables\\operators.txt",
      "input\\tables\\letters.txt",
      "input\\tables\\digits.txt",
      "input\\tables\\operatorCharacters.txt",
      "input\\tables\\brackets.txt",
      "input\\tables\\separators.txt",
      "input\\tables\\whitespaces.txt",
      "input\\tables\\identifiers.txt",
      "input\\tables\\literals.txt"
   );
   if (!tables) {
      io::logError("Error: can't proceed with scanning because Tables instance doesn't exist.", std::cerr);
      return -1;
   }

   /*       Создание сканера и запуск лексического анализа.        */
   scanner::Scanner scanner(*tables);
   std::vector<Token> tokens = scanner.scan("input\\test-code.txt", errorStream);

   /*       Вывод таблицы токенов и символов в соответствующие файлы.        */
   io::writeTokens("tokens.txt", tokens);
   io::writeAll(*tables,
      "output\\tables\\keywords.txt",
      "output\\tables\\operators.txt",
      "output\\tables\\letters.txt",
      "output\\tables\\digits.txt",
      "output\\tables\\operatorCharacters.txt",
      "output\\tables\\brackets.txt",
      "output\\tables\\separators.txt",
      "output\\tables\\whitespaces.txt",
      "output\\tables\\identifiers.txt",
      "output\\tables\\literals.txt");
   ///*          Обработка критических ошибок.           */
   //catch (std::exception const& e) {
   //   std::string s = e.what();
   //   errorStream << e.what() << std::endl;
   //}
   return 0;
}
