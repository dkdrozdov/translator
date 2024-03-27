#pragma once

#include "common_includes.h"
#include "table.h"
#include "fstream"
#include "io.h"
#include "token.h"
#include "dfa.h"

namespace scanner {

   /*
      Описывает лексический анализатор (сканер).
      Состоит из связки таблиц и ДКА.
   */
   struct Scanner {
      Tables tables;          // Связка таблиц.
      DFA dfa;           // ДКА.
      io::IO& io;             // Ссылка на объект io для вывода ошибок.
      int currentLine;        // Номер текущей строки.
      int currentColumn;      // Номер текущего символа в строке.
      int chainStartColumn;   // Номер символа, с которого начинается текущая цепочка.

      Scanner(Tables& _tables, io::IO& _io);

      /*
         Описывает состояния ДКА.
      */
      enum class DFAState {
         START, EXCLAMATION, EQUALS, OPERATOR, IDENTIFIER, LITERAL, SEPARATOR, BRACKET, FAIL, COUNT
      };

      /*
         Вспомогательная функция для устрожения типизации параметров (уменьшает риск ошибки).
      */
      static void setDFATransition(DFA& dfa, DFAState state, CharacterClass characterClass, DFAState nextState);

      /*
         Строит ДКА по связке таблиц tables.
         Возвращает детерминированный конечный автомат DFA.
      */
      static DFA buildDFA(Tables& tables);;

      /*
         Производит попытку создания токена, соответствующего состоянию ДКА state и символу с именем name, найденного на строке line и столбце column.
         Возвращает указатель на созданный токен, или nullptr в случае ошибки.
      */
      Token* createToken(DFAState state, std::string name);

      /*
         Запускает алгоритм лексического анализа над исходным кодом в файле inputPath,
         преобразуя лексические единицы кода в токены в порядке их появления.
         Некритичные ошибки, связанные с недопустимыми символами, записываются в поток ошибок errorStream.
         Возвращает список полученных токенов.
      */
      std::vector<Token> scan(std::string inputPath);
   };
}