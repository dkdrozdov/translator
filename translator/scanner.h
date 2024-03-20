#pragma once

#include "common_includes.h";
#include "table.h"
#include "fstream"
#include "io.h"
#include "token.h"

namespace scanner {

   struct DFA {
   private:
      const int stateCount;
      const int transitionCharacterCount;

      int** matrix;

      int currentState = 0;

   public:
      DFA(int _stateCount, int _transitionCharacterCount, int failState) : stateCount(_stateCount), transitionCharacterCount(_transitionCharacterCount) {
         matrix = new int* [_stateCount];

         for (int i = 0; i < stateCount; i++) {
            matrix[i] = new int[_transitionCharacterCount];
            for (int j = 0; j < transitionCharacterCount; j++) {
               matrix[i][j] = failState;
            }
         }
      }

      ~DFA() {
         for (int i = 0; i < stateCount; i++) {
            delete[] matrix[i];
         }
         delete[] matrix;
      }

      void setTransition(int state, int transitionCharacter, int nextState) {
         matrix[state][transitionCharacter] = nextState;
      }

      void transition(int transitionCharacter) {
         if (transitionCharacter >= transitionCharacterCount)
            throw std::out_of_range("Illegal transition character: " + transitionCharacter);

         currentState = matrix[currentState][transitionCharacter];
      }

      int getState() const {
         return currentState;
      }

      void setState(int state) {
         currentState = state;
      }
   };

   struct Scanner {
      Tables tables;
      DFA dfa;

      enum DFAState {
         START, EXCLAMATION, EQUALS, OPERATOR, IDENTIFIER, LITERAL, SEPARATOR, BRACKET, FAIL, COUNT
      };

      static void setDFATransition(DFA& dfa, DFAState state, CharacterClass characterClass, DFAState nextState) {
         dfa.setTransition(state, characterClass, nextState);
      }

      static DFA buildDFA(Tables& tables) {
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
      };

      Scanner(Tables& _tables) : tables(_tables), dfa(buildDFA(tables)) {}

      Token createToken(DFAState state, std::string name, int line, int column) {
         switch (state)
         {
         case IDENTIFIER:
         {
            if (tables.keywords.contains(name)) {
               return Token(KEYWORD_TOKEN_TYPE, tables.keywords.find(name), line, column, name, &tables);
            }

            TableEntry* entry = tables.identifiers->find(name);
            if (entry != nullptr)
               return Token(IDENTIFIER_TOKEN_TYPE, entry->tableIndex, line, column, name, &tables);
            else
               return Token(IDENTIFIER_TOKEN_TYPE, tables.identifiers->add(Attributes::IntVariableAttributes(name, 0)), line, column, name, &tables);

            break;
         }
         case LITERAL:
         {
            int value;

            try {
               value = std::stoi(name);
            }
            catch (...) {
               throw std::exception(("Invalid literal: " + name).c_str());
            }

            std::string formattedName = "%d" + name;
            TableEntry* entry = tables.literals->find(formattedName);

            if (entry != nullptr)
               return Token(LITERAL_TOKEN_TYPE, entry->tableIndex, line, column, formattedName, &tables);
            else
               return Token(LITERAL_TOKEN_TYPE, tables.literals->add(Attributes::IntLiteralAttributes(value)), line, column, formattedName, &tables);

            break;
         }
         case EQUALS:   // < умышленный пропуск break: случай EQUALS является частью OPERATOR.
         case OPERATOR:
         {
            if (tables.operators.contains(name))
               return Token(OPERATOR_TOKEN_TYPE, tables.operators.find(name), line, column, name, &tables);
            else throw std::exception(("Error creating token. Couldn't find operator " + name).c_str());

            break;
         }
         case SEPARATOR:
         {
            if (tables.separators.contains(name))
               return Token(SEPARATOR_TOKEN_TYPE, tables.separators.find(name), line, column, name, &tables);
            else throw std::exception(("Error creating token. Couldn't find separator " + name).c_str());

            break;
         }
         case BRACKET:
         {
            if (tables.brackets.contains(name))
               return Token(BRACKET_TOKEN_TYPE, tables.brackets.find(name), line, column, name, &tables);
            else throw std::exception(("Error creating token. Couldn't find bracket " + name).c_str());

            break;
         }
         default:
            throw std::exception("Error creating token. DFA failure: " + state);
            break;
         }
      }

      std::vector<Token> scan(std::string inputPath, std::ostream& errorStream) {
         std::ifstream ifs(inputPath);
         std::vector<Token> tokens;
         char c;
         std::string chain;
         int previousState = START;
         dfa.setState(START);
         int line = 0;
         int character = 0;
         int startingCharacter = 0;

         bool tokenize = false;
         bool discard = false;

         while (ifs.get(c)) {
            character++;

            if (c == '\n') {
               line++;
               character = 0;
            }

            CharacterClass characterClass = tables.classify(c);

            if (characterClass == WHITESPACE_CHARACTER) {
               discard = true;
               tokenize = true;
            }

            if (characterClass == INVALID_CHARACTER)
            {
               errorStream << "Invalid character: " + c << " at line " << line << "; character " << character << "." << std::endl;
               continue;
            }

            try {
               dfa.transition(characterClass);
            }
            catch (std::out_of_range) {
               ifs.close();
               throw std::exception("Error following DFA transition: state out of range.");
            }

            if (dfa.getState() == FAIL && chain != "")   // ДКА в состоянии неудачи, что означает что цепочка завершена или недопустима.
            {
               try {
                  tokens.push_back(createToken((DFAState)previousState, chain, line, startingCharacter)); // Проверка цепочки и добавление нового токена.
               }
               catch (std::exception const& e) {
                  errorStream << e.what() << " at line " << line << "; character " << character << "." << std::endl;
               }

               dfa.setState(START); // Сброс ДКА.
               chain = "";          // Сброс цепочки.
               startingCharacter = character;
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
               startingCharacter = character;
            }
            else
               previousState = dfa.getState();
         }
         if (chain != "") {   // Цикл завершился досрочно, попытка сделать токен из последней цепочки.
            try {
               tokens.push_back(createToken((DFAState)previousState, chain, line, startingCharacter));
            }
            catch (std::exception const& e) {
               errorStream << e.what() << " at line " << line << "; character " << character << "." << std::endl;
            }
         }

         ifs.close();

         return tokens;
      }
   };
}