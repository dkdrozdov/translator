#pragma once

#include <fstream>
#include "common_includes.h"
#include "table.h"
#include "token.h"

namespace io {
   /*
      ������ ���� path � ������ �� ���� ����������� �������.
   */
   StaticTable readStaticTable(std::string path);

   /*
      ������ ���� path � ������ �� ���� ������������ �������.
   */
   MutableTable* readMutableTable(std::string path);

   /*
      ���������� � ���� path ����������� �������.
   */
   void writeStaticTable(std::string path, StaticTable& table);

   /*
      ���������� � ���� path ������������ �������.
   */
   void writeMutableTable(std::string path, MutableTable& table);

   /*
      ���������� ������ � ���� path.
   */
   void writeTokens(std::string path, std::vector<Token> tokens);

   /*
      ������ ������� �� ������.
      ���������� ������ ������ Tables.
   */
   static Tables readTables(std::string keywordsPath, std::string operatorsPath, std::string lettersPath,
      std::string digitsPath, std::string operatorCharactersPath, std::string bracketsPath,
      std::string separatorsPath, std::string whitespacesPath,
      std::string identifiersPath, std::string literalsPath)
   {
      return Tables(readStaticTable(keywordsPath),
         readStaticTable(operatorsPath),
         readStaticTable(lettersPath),
         readStaticTable(digitsPath),
         readStaticTable(operatorCharactersPath),
         readStaticTable(bracketsPath),
         readStaticTable(separatorsPath),
         readStaticTable(whitespacesPath),
         readMutableTable(identifiersPath),
         readMutableTable(literalsPath));
   }

   /*
      ���������� ������� �� ������ tables � �����.
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
