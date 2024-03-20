#pragma once

#include "common_includes.h"
#include "table.h"

enum TokenType {
   KEYWORD_TOKEN_TYPE,
   OPERATOR_TOKEN_TYPE,
   SEPARATOR_TOKEN_TYPE,
   IDENTIFIER_TOKEN_TYPE,
   LITERAL_TOKEN_TYPE,
   BRACKET_TOKEN_TYPE,
   INVALID_TOKEN_TYPE
};

struct Token {
private:
   TokenType type;

   Tables* tables; // ������ �� ������� (����������� ���� ��� ���������� �������, ��������� ������� ��� ���).
   int address;   // ������ � ������� (������ ��� ����������� ������)

public:

   TokenType getType() const { return type; }


   /*
      ���������� ������, ���� ��������� �� ������������ �������.
   */
   bool isInMutableTable() const {
      return type == IDENTIFIER_TOKEN_TYPE || type == LITERAL_TOKEN_TYPE;
   }

   /*
      ����������� ����, ��������� ������� ����� �������������� � �������� ����������� (��� ������������ ������)
   */
   int getAddress() {
      if (!isInMutableTable()) return address;
      try
      {
         switch (type)
         {
         case IDENTIFIER_TOKEN_TYPE:
         {
            return tables->identifiers->find(name)->tableIndex;
            break;
         }
         case LITERAL_TOKEN_TYPE:
         {
            return tables->literals->find(name)->tableIndex;

            break;
         }
         }
      }
      catch (...)
      {
         throw std::exception("Error: couldn't get token address.");
      }
      throw std::exception("Error: couldn't get token address.");
   };

   // ��������������� ����:
   int line;
   int column;
   std::string name;

   Token(TokenType _type, int _address, int, int, std::string, Tables*);
};