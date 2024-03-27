#pragma once

#include <fstream>
#include "common_includes.h"
#include "table.h"
#include "token.h"

namespace io {
   /*
      Хранит поток ошибок errorStream, отвечает за ввод и вывод.
   */
   class IO {
      std::ofstream errorStream;

   public:
      IO(std::string errorMessagesFilePath) : errorStream(errorMessagesFilePath) {}
      /*
         Сообщает об ошибке в errorStream.
      */
      void logError(std::string message);
      /*
         Сообщает об ошибке открытия файла name в errorStream.
      */
      void logFileOpeningError(std::string name);
      /*
         Записывает лексическую ошибку message, находящуюся на строке line и столбце column в поток errorStream.
      */
      void logLexicalError(std::string message, int line, int column);

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
      Tables* readTables(std::string keywordsPath, std::string operatorsPath, std::string lettersPath,
         std::string digitsPath, std::string operatorCharactersPath, std::string bracketsPath,
         std::string separatorsPath, std::string whitespacesPath)
      {
         StaticTable* keywords(readStaticTable(keywordsPath));
         StaticTable* operators(readStaticTable(operatorsPath));
         StaticTable* letters(readStaticTable(lettersPath));
         StaticTable* digits(readStaticTable(digitsPath));
         StaticTable* operatorCharacters(readStaticTable(operatorCharactersPath));
         StaticTable* brackets(readStaticTable(bracketsPath));
         StaticTable* separators(readStaticTable(separatorsPath));
         StaticTable* whitespaces(readStaticTable(whitespacesPath));

         if (!(keywords && operators && letters && digits && operatorCharacters && brackets &&
            separators && whitespaces)) {
            logError("Error: can't create new Table instance because one or more of the tables is absent!");
            return nullptr;
         }

         return new Tables(*keywords, *operators, *letters, *digits, *operatorCharacters,
            *brackets, *separators, *whitespaces);
      }

      /*
         Записывает таблицы из связки tables в файлы.
      */
      void writeAll(Tables& tables, std::string keywordsPath,
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
         writeStaticTable(keywordsPath, tables.keywords);
         writeStaticTable(operatorsPath, tables.operators);
         writeStaticTable(lettersPath, tables.letters);
         writeStaticTable(digitsPath, tables.digits);
         writeStaticTable(operatorCharactersPath, tables.operatorCharacters);
         writeStaticTable(bracketsPath, tables.brackets);
         writeStaticTable(separatorsPath, tables.separators);
         writeStaticTable(whitespacesPath, tables.whitespaces);
         writeMutableTable(identifiersPath, *(tables.identifiers));
         writeMutableTable(literalsPath, *(tables.literals));
      }
   };

}
