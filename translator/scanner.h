#pragma once

#include "common_includes.h";
#include "table.h"
#include "fstream"
#include "io.h"
#include "token.h"
#include "dfa.h"

namespace scanner {

   /*
      ��������� ����������� ���������� (������).
      ������� �� ������ ������ � ���.
   */
   struct Scanner {
      Tables tables; // ������ ������.
      DFA dfa;       // ���.

      Scanner(Tables& _tables);

      /*
         ��������� ��������� ���.
      */
      enum DFAState {
         START, EXCLAMATION, EQUALS, OPERATOR, IDENTIFIER, LITERAL, SEPARATOR, BRACKET, FAIL, COUNT
      };

      /*
         ��������������� ������� ��� ���������� ��������� ���������� (��������� ���� ������).
      */
      static void setDFATransition(DFA& dfa, DFAState state, CharacterClass characterClass, DFAState nextState);

      /*
         ������ ��� �� ������ ������ tables.
         ���������� ����������������� �������� ������� DFA.
      */
      static DFA buildDFA(Tables& tables);;

      /*
         ���������� ������� �������� ������, ���������������� ��������� ��� state � ������� � ������ name, ���������� �� ������ line � ������� column.
         ���������� ��������� ����� Token.
      */
      Token createToken(DFAState state, std::string name, int line, int column);

      /*
         ��������� �������� ������������ ������� ��� �������� ����� � ����� inputPath, 
         ���������� ����������� ������� ���� � ������ � ������� �� ���������.
         ����������� ������, ��������� � ������������� ���������, ������������ � ����� ������ errorStream.
         ���������� ������ ���������� �������.
      */
      std::vector<Token> scan(std::string inputPath, std::ostream& errorStream);
   };
}