#pragma once

#include "common_includes.h"

/*
   ��������� ����������������� �������� ������� (���).
   ������� �� ������� ��������� � �������� ���������.
*/
struct DFA {
private:
   const int stateCount;               // ���������� ���������.
   const int transitionCharacterCount; // ���������� �������� ��������.

   int** matrix;  // ������� ���������.

   int currentState = 0;   // ������� ���������.

public:

   /*
      ������ ��� � ����������� ��������� _stateCount � ����������� �������� �������� _transitionCharacterCount.
      �������� failState ����� ��������� �������� �� ���������.
   */
   DFA(int _stateCount, int _transitionCharacterCount, int failState);

   ~DFA();

   /*
      ������������� ������� �������� �� ��������� state � ��������� nextState �� ������� transitionCharacter.
   */
   void setTransition(int state, int transitionCharacter, int nextState);

   /*
      ���������� ������� �� ������� transitionCharacter, ������������� ��� ������ ��������� currentState �����.
   */
   void transition(int transitionCharacter);

   int getState() const;

   void setState(int state);
};