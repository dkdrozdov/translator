#include "scanner.h"

void scanner::Scanner::setDFATransition(DFA& dfa, DFAState state, CharacterClass characterClass, DFAState nextState) {
   dfa.setTransition(state, characterClass, nextState);
}

DFA scanner::Scanner::buildDFA(Tables& tables) {
   DFA df(DFAState::COUNT, CharacterClass::COUNT_CHARACTER, FAIL);

   setDFATransition(df, START, OPERATOR_CHARACTER_CHARACTER, OPERATOR);
   setDFATransition(df, START, EQUAL_SIGN_CHARACTER, EQUALS);
   setDFATransition(df, EQUALS, EQUAL_SIGN_CHARACTER, OPERATOR);
   setDFATransition(df, START, EXCLAMATION_MARK_CHARACTER, EXCLAMATION);
   setDFATransition(df, EXCLAMATION, EQUAL_SIGN_CHARACTER, OPERATOR);
   setDFATransition(df, START, LETTER_CHARACTER, IDENTIFIER);
   setDFATransition(df, IDENTIFIER, LETTER_CHARACTER, IDENTIFIER);
   setDFATransition(df, IDENTIFIER, DIGIT_CHARACTER, IDENTIFIER);
   setDFATransition(df, START, DIGIT_CHARACTER, LITERAL);
   setDFATransition(df, START, SEPARATOR_CHARACTER, SEPARATOR);
   setDFATransition(df, START, BRACKET_CHARACTER, BRACKET);

   return df;
}

scanner::Scanner::Scanner(Tables& _tables, io::IO& _io) : tables(_tables), io(_io), dfa(buildDFA(tables)), currentColumn(1), currentLine(1), chainStartColumn(1) {}

Token* scanner::Scanner::createToken(DFAState state, std::string name) {
   switch (state)
   {
   case IDENTIFIER:
   {
      if (tables.keywords.contains(name))
         return new Token(KEYWORD_TOKEN_TYPE, tables.keywords.find(name), currentLine, currentColumn, name, &tables);

      TableEntry* entry = tables.identifiers->find(name);

      if (entry != nullptr)
         return new Token(IDENTIFIER_TOKEN_TYPE, entry->tableIndex, currentLine, currentColumn, name, &tables);
      else
         return new Token(IDENTIFIER_TOKEN_TYPE, tables.identifiers->add(Attributes::IntVariableAttributes(name, 0)), currentLine, currentColumn, name, &tables);

      return 0;   // For some reason IDENTIFIER state cannot cause any errors.
      break;
   }
   case LITERAL:
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
         return new Token(LITERAL_TOKEN_TYPE, entry->tableIndex, currentLine, currentColumn, formattedName, &tables);
      else
         return new Token(LITERAL_TOKEN_TYPE, tables.literals->add(Attributes::IntLiteralAttributes(value)), currentLine, currentColumn, formattedName, &tables);

      break;
   }
   case EQUALS:   // < умышленный пропуск break: случай EQUALS является частью OPERATOR.
   case OPERATOR:
   {
      if (tables.operators.contains(name))
         return new Token(OPERATOR_TOKEN_TYPE, tables.operators.find(name), currentLine, currentColumn, name, &tables);
      else
      {
         io.logLexicalError("Error creating token. Couldn't find operator "s + name, currentLine, currentColumn);
         return nullptr;
      }

      break;
   }
   case SEPARATOR:
   {
      if (tables.separators.contains(name))
         return new Token(SEPARATOR_TOKEN_TYPE, tables.separators.find(name), currentLine, currentColumn, name, &tables);
      else
      {
         io.logLexicalError("Error creating token. Couldn't find separator "s + name, currentLine, currentColumn);
         return nullptr;
      }
      break;
   }
   case BRACKET:
   {
      if (tables.brackets.contains(name))
         return new Token(BRACKET_TOKEN_TYPE, tables.brackets.find(name), currentLine, currentColumn, name, &tables);
      else
      {
         io.logLexicalError("Error creating token. Couldn't find bracket "s + name, currentLine, currentColumn);
         return nullptr;
      }
      break;
   }
   default:
   {
      io.logLexicalError("Error creating token. Invalid DFA state: "s + std::to_string(state), currentLine, currentColumn);
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
   int previousState = START;
   dfa.setState(START);

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

      if (characterClass == WHITESPACE_CHARACTER) {
         discard = true;
         tokenize = true;
      }

      if (characterClass == INVALID_CHARACTER)
      {
         io.logLexicalError("Invalid character: "s + c, currentLine, currentColumn);
         continue;
      }

      // try/catch removed: cannot recover from this exception
      dfa.transition(characterClass);

      if (dfa.getState() == FAIL && chain != "")   // ДКА в состоянии неудачи, что означает что цепочка завершена или недопустима.
      {
         Token* newToken = createToken((DFAState)previousState, chain);
         if (newToken)
            tokens.push_back(*newToken); // Проверка цепочки и добавление нового токена.


         dfa.setState(START); // Сброс ДКА.
         chain = "";          // Сброс цепочки.
         startingCharacter = currentColumn;
         tokenize = true;
      }

      if (!discard)
      {
         chain += c;
         if (tokenize) {
            dfa.transition(characterClass);
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
         dfa.setState(START);
         previousState = START;
         startingCharacter = currentColumn;
      }
      else
         previousState = dfa.getState();
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
