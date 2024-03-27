#include "scanner.h"

void scanner::Scanner::setDFATransition(DFA& dfa, DFAState state, CharacterClass characterClass, DFAState nextState) {
   dfa.setTransition((int)state, (int)characterClass, (int)nextState);
}

DFA scanner::Scanner::buildDFA(Tables& tables) {
   DFA df((int)DFAState::COUNT, (int)CharacterClass::COUNT, (int)DFAState::FAIL);

   setDFATransition(df, DFAState::START, CharacterClass::OPERATOR_CHARACTER, DFAState::OPERATOR);
   setDFATransition(df, DFAState::START, CharacterClass::EQUAL_SIGN, DFAState::EQUALS);
   setDFATransition(df, DFAState::EQUALS, CharacterClass::EQUAL_SIGN, DFAState::OPERATOR);
   setDFATransition(df, DFAState::START, CharacterClass::EXCLAMATION_MARK, DFAState::EXCLAMATION);
   setDFATransition(df, DFAState::EXCLAMATION, CharacterClass::EQUAL_SIGN, DFAState::OPERATOR);
   setDFATransition(df, DFAState::START, CharacterClass::LETTER, DFAState::IDENTIFIER);
   setDFATransition(df, DFAState::IDENTIFIER, CharacterClass::LETTER, DFAState::IDENTIFIER);
   setDFATransition(df, DFAState::IDENTIFIER, CharacterClass::DIGIT, DFAState::IDENTIFIER);
   setDFATransition(df, DFAState::START, CharacterClass::DIGIT, DFAState::LITERAL);
   setDFATransition(df, DFAState::START, CharacterClass::SEPARATOR, DFAState::SEPARATOR);
   setDFATransition(df, DFAState::START, CharacterClass::BRACKET, DFAState::BRACKET);

   return df;
}

scanner::Scanner::Scanner(Tables& _tables, io::IO& _io) : tables(_tables), io(_io), dfa(buildDFA(tables)), currentColumn(1), currentLine(1), chainStartColumn(1) {}

Token* scanner::Scanner::createToken(DFAState state, std::string name) {
   switch (state)
   {
   case DFAState::IDENTIFIER:
   {
      if (tables.keywords.contains(name))
         return new Token(TokenType::KEYWORD, tables.keywords.find(name), currentLine, currentColumn, name, &tables);

      TableEntry* entry = tables.identifiers->find(name);

      if (entry != nullptr)
         return new Token(TokenType::IDENTIFIER, entry->tableIndex, currentLine, currentColumn, name, &tables);
      else
         return new Token(TokenType::IDENTIFIER, tables.identifiers->add(Attributes::IntVariableAttributes(name, 0)), currentLine, currentColumn, name, &tables);

      return 0;   // For some reason IDENTIFIER state cannot cause any errors.
      break;
   }
   case DFAState::LITERAL:
   {
      int value;

      try {
         value = std::stoi(name);
      }
      catch (...) {
         io.logLexicalError("Invalid literal: "s + name, currentLine, currentColumn);
         return nullptr;
      }

      std::string formattedName = "%d"s + name;
      TableEntry* entry = tables.literals->find(formattedName);

      if (entry != nullptr)
         return new Token(TokenType::LITERAL, entry->tableIndex, currentLine, currentColumn, formattedName, &tables);
      else
         return new Token(TokenType::LITERAL, tables.literals->add(Attributes::IntLiteralAttributes(value)), currentLine, currentColumn, formattedName, &tables);

      break;
   }
   case DFAState::EQUALS:   // < умышленный пропуск break: случай EQUALS является частью OPERATOR.
   case DFAState::OPERATOR:
   {
      if (tables.operators.contains(name))
         return new Token(TokenType::OPERATOR, tables.operators.find(name), currentLine, currentColumn, name, &tables);
      else
      {
         io.logLexicalError("Error creating token. Couldn't find operator "s + name, currentLine, currentColumn);
         return nullptr;
      }

      break;
   }
   case DFAState::SEPARATOR:
   {
      if (tables.separators.contains(name))
         return new Token(TokenType::SEPARATOR, tables.separators.find(name), currentLine, currentColumn, name, &tables);
      else
      {
         io.logLexicalError("Error creating token. Couldn't find separator "s + name, currentLine, currentColumn);
         return nullptr;
      }
      break;
   }
   case DFAState::BRACKET:
   {
      if (tables.brackets.contains(name))
         return new Token(TokenType::BRACKET, tables.brackets.find(name), currentLine, currentColumn, name, &tables);
      else
      {
         io.logLexicalError("Error creating token. Couldn't find bracket "s + name, currentLine, currentColumn);
         return nullptr;
      }
      break;
   }
   default:
   {
      io.logLexicalError("Error creating token. Invalid DFA state: "s + std::to_string((int)state), currentLine, currentColumn);
      return nullptr;

      break;
   }
   }

   return nullptr;
}

std::vector<Token> scanner::Scanner::scan(std::string inputPath) {
   std::ifstream ifs(inputPath);
   std::vector<Token> tokens;

   if (!ifs)
   {
      io.logFileOpeningError(inputPath);
      return tokens;
   }

   char c;
   std::string chain;
   DFAState previousState = DFAState::START;
   dfa.setState((int)DFAState::START);

   /*    Позиция цепочки в коде     */
   currentLine = 1;
   currentColumn = 0;
   int startingCharacter = 1;

   bool tokenize = false;
   bool discard = false;

   while (ifs.get(c)) {
      currentColumn++;

      if (c == '\n') {
         currentLine++;
         currentColumn = 1;
      }

      CharacterClass characterClass = tables.classify(c);

      if (characterClass == CharacterClass::WHITESPACE) {
         discard = true;
         tokenize = true;
      }

      if (characterClass == CharacterClass::INVALID)
      {
         io.logLexicalError("Invalid character: "s + c, currentLine, currentColumn);
         continue;
      }

      // try/catch removed: cannot recover from this exception
      dfa.transition((int)characterClass);

      if (dfa.getState() == (int)DFAState::FAIL && chain != "")   // ДКА в состоянии неудачи, что означает что цепочка завершена или недопустима.
      {
         Token* newToken = createToken((DFAState)previousState, chain);
         if (newToken)
            tokens.push_back(*newToken); // Проверка цепочки и добавление нового токена.


         dfa.setState((int)DFAState::START); // Сброс ДКА.
         chain = "";          // Сброс цепочки.
         startingCharacter = currentColumn;
         tokenize = true;
      }

      if (!discard)
      {
         chain += c;
         if (tokenize) {
            dfa.transition((int)characterClass);
            tokenize = false;
         }
      }
      else
      {
         discard = false;
         tokenize = false;
      }

      if (chain == "")
      {
         dfa.setState((int)DFAState::START);
         previousState = DFAState::START;
         startingCharacter = currentColumn;
      }
      else
         previousState = (DFAState)dfa.getState();
   }
   if (chain != "") {   // Цикл завершился досрочно, попытка сделать токен из последней цепочки.
      // try/catch removed: createToken should handle errors and log them
      {
         Token* newToken = createToken((DFAState)previousState, chain);

         if (newToken)
            tokens.push_back(*newToken);
      }
   }

   ifs.close();

   return tokens;
}
