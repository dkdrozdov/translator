#include <iostream>
#include "common_includes.h"
#include "table.h"
#include "io.h"
#include "scanner.h"

static const std::string errorMessagesFilePath = "output\\error-messages.txt";

static const std::string keywordsTablePath = "input\\tables\\keywords";
static const std::string operatorsTablePath = "input\\tables\\operators";
static const std::string lettersTablePath = "input\\tables\\letters";
static const std::string digitsTablePath = "input\\tables\\digits";
static const std::string operatorCharactersTablePath = "input\\tables\\operatorCharacters";
static const std::string bracketsTablePath = "input\\tables\\brackets";
static const std::string separatorsTablePath = "input\\tables\\separators";
static const std::string whitespacesTablePath = "input\\tables\\whitespaces";

static const std::string keywordsTableOutputPath = "output\\tables\\keywords.csv";
static const std::string operatorsTableOutputPath = "output\\tables\\operators.csv";
static const std::string lettersTableOutputPath = "output\\tables\\letters.csv";
static const std::string digitsTableOutputPath = "output\\tables\\digits.csv";
static const std::string operatorCharactersTableOutputPath = "output\\tables\\operatorCharacters.csv";
static const std::string bracketsTableOutputPath = "output\\tables\\brackets.csv";
static const std::string separatorsTableOutputPath = "output\\tables\\separators.csv";
static const std::string whitespacesTableOutputPath = "output\\tables\\whitespaces.csv";
static const std::string identifiersTableOutputPath = "output\\tables\\identifiers.csv";
static const std::string literalsTableOutputPath = "output\\tables\\literals.csv";

static const std::string testCodePath = "input\\test-code.c";
static const std::string tokensTablePath = "output\\tokens.csv";

int main()
{
   // Вариант 5:	
   //    Подмножество языка С++ включает:
   //    • данные типа int;
   //    • инструкции описания переменных;
   //    • операторы присваивания, switch  любой вложенности и в любой последовательности;
   //    • операции + , –, *, = =, != , < .

   io::IO io(errorMessagesFilePath);

   /*       Чтение вводных таблиц.        */
   Tables* tables = io.readTables(
      keywordsTablePath,
      operatorsTablePath,
      lettersTablePath,
      digitsTablePath,
      operatorCharactersTablePath,
      bracketsTablePath,
      separatorsTablePath,
      whitespacesTablePath);

   if (!tables) {
      io.logError("Error: can't proceed with scanning because Tables instance doesn't exist.");
      return -1;
   }

   /*       Создание сканера и запуск лексического анализа.        */
   scanner::Scanner scanner(*tables, io);
   std::vector<Token> tokens = scanner.scan(testCodePath);

   /*       Вывод таблицы токенов и символов в соответствующие файлы.        */
   io.writeTokens(tokensTablePath, tokens);
   io.writeAll(*tables,
      keywordsTableOutputPath,
      operatorsTableOutputPath,
      lettersTableOutputPath,
      digitsTableOutputPath,
      operatorCharactersTableOutputPath,
      bracketsTableOutputPath,
      separatorsTableOutputPath,
      whitespacesTableOutputPath,
      identifiersTableOutputPath,
      literalsTableOutputPath);

   return 0;
}