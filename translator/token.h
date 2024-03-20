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

   Tables* tables; // —сылка на таблицы (необходима€ мера дл€ нахождени€ индекса, поскольку таблицы имЄн нет).
   int address;   // »ндекс в таблице (только дл€ статических таблиц)

public:

   TokenType getType() const { return type; }


   /*
      ¬озвращает истину, если ссылаетс€ на динамическую таблицу.
   */
   bool isInMutableTable() const {
      return type == IDENTIFIER_TOKEN_TYPE || type == LITERAL_TOKEN_TYPE;
   }

   /*
      ¬ычисл€емое поле, поскольку индексы могут рехешироватьс€ в процессе токенизации (дл€ динамических таблиц)
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

   // вспомогательные пол€:
   int line;
   int column;
   std::string name;

   Token(TokenType _type, int _address, int, int, std::string, Tables*);
};