#pragma once

#include "common_includes.h"

/*
   Описывает детерминированный конечный автомат (ДКА).
   Состоит из матрицы переходов и текущего состояния.
*/
struct DFA {
private:
   const int stateCount;               // Количество состояний.
   const int transitionCharacterCount; // Количество символов перехода.

   int** matrix;  // Матрица переходов.

   int currentState = 0;   // Текущее состояние.

public:

   /*
      Создаёт ДКА с количеством состояний _stateCount и количеством символов перехода _transitionCharacterCount.
      Параметр defaultState задаёт состояние перехода по умолчанию.
   */
   DFA(int _stateCount, int _transitionCharacterCount, int defaultState);

   ~DFA();

   /*
      Устанавливает правило перехода от состояния state к состоянию nextState по символу transitionCharacter.
   */
   void setTransition(int state, int transitionCharacter, int nextState);

   /*
      Производит переход по символу transitionCharacter, перезаписывая своё старое состояние currentState новым.
   */
   void transition(int transitionCharacter);

   int getState() const;

   void setState(int state);
};
