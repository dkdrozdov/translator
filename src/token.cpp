#include "token.h"


TokenType Token::getType() const { return type; }

bool Token::isInMutableTable() const {
   return type == TokenType::IDENTIFIER || type == TokenType::LITERAL;
}

int Token::getAddress() {
   if (!isInMutableTable()) return address;
   try
   {
      switch (type)
      {
      case TokenType::IDENTIFIER:
      {
         return tables->identifiers->find(name)->tableIndex;
         break;
      }
      case TokenType::LITERAL:
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
}
