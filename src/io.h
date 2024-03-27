#pragma once

#include <fstream>
#include "common_includes.h"
#include "table.h"
#include "token.h"

namespace io {
   /*
      Сообщает об ошибке в errorStream.
   */
   void logError(std::string message, std::ostream& errorStream);
   /*
      Сообщает об ошибке открытия файла name в errorStream.
   */
   void logFileOpeningError(std::string name, std::ostream& errorStream);
   /*
      Записывает лексическую ошибку message, находящуюся на строке line и столбце column в поток errorStream.
   */
   void logLexicalError(std::string message, int line, int column, std::ostream& errorStream);

   /*
      Читает файл path и строит по нему статическую таблицу.
   */
   StaticTable* readStaticTable(std::string path);

   /*
      Читает файл path и строит по нему динамическую таблицу.
   */
   MutableTable* readMutableTable(std::string path);

   /*
      Записывает в файл path статическую таблицу.
   */
   void writeStaticTable(std::string path, StaticTable& table);

   /*
      Записывает в файл path динамическую таблицу.
   */
   void writeMutableTable(std::string path, MutableTable& table);

   /*
      Записывает токены в файл path.
   */
   void writeTokens(std::string path, std::vector<Token> tokens);

   /*
      Читает таблицы из файлов.
      Возвращает связку таблиц Tables.
   */
   static Tables* readTables(std::string keywordsPath, std::string operatorsPath, std::string lettersPath,
      std::string digitsPath, std::string operatorCharactersPath, std::string bracketsPath,
      std::string separatorsPath, std::string whitespacesPath,
      std::string identifiersPath, std::string literalsPath)
   {
      StaticTable* keywords(readStaticTable(keywordsPath));
      StaticTable* operators(readStaticTable(operatorsPath));
      StaticTable* letters(readStaticTable(lettersPath));
      StaticTable* digits(readStaticTable(digitsPath));
      StaticTable* operatorCharacters(readStaticTable(operatorCharactersPath));
      StaticTable* brackets(readStaticTable(bracketsPath));
      StaticTable* separators(readStaticTable(separatorsPath));
      StaticTable* whitespaces(readStaticTable(whitespacesPath));
      MutableTable* identifiers(readMutableTable(identifiersPath));
      MutableTable* literals(readMutableTable(literalsPath));

      if (!(keywords && operators && letters && digits && operatorCharacters && brackets &&
         separators && whitespaces && identifiers && literals)) {
         logError("Error: can't create new Table instance because one or more of the tables is absent!", std::cerr);
         return nullptr;
      }

      return new Tables(*keywords, *operators, *letters, *digits, *operatorCharacters,
         *brackets, *separators, *whitespaces, identifiers, literals);
   }

   /*
      Записывает таблицы из связки tables в файлы.
   */
   static void writeAll(Tables& tables, std::string keywordsPath,
      std::string operatorsPath,
      std::string lettersPath,
      std::string digitsPath,
      std::string operatorCharactersPath,
      std::string bracketsPath,
      std::string separatorsPath,
      std::string whitespacesPath,
      std::string identifiersPath,
      std::string literalsPath)
   {
      io::writeStaticTable(keywordsPath, tables.keywords);
      io::writeStaticTable(operatorsPath, tables.operators);
      io::writeStaticTable(lettersPath, tables.letters);
      io::writeStaticTable(digitsPath, tables.digits);
      io::writeStaticTable(operatorCharactersPath, tables.operatorCharacters);
      io::writeStaticTable(bracketsPath, tables.brackets);
      io::writeStaticTable(separatorsPath, tables.separators);
      io::writeStaticTable(whitespacesPath, tables.whitespaces);
      io::writeMutableTable(identifiersPath, *(tables.identifiers));
      io::writeMutableTable(literalsPath, *(tables.literals));
   }
}
