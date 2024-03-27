#include "dfa.h"

DFA::DFA(int _stateCount, int _transitionCharacterCount, int defaultState) : stateCount(_stateCount), transitionCharacterCount(_transitionCharacterCount) {
   matrix = new int* [_stateCount];

   for (int i = 0; i < stateCount; i++) {
      matrix[i] = new int[_transitionCharacterCount];
      for (int j = 0; j < transitionCharacterCount; j++) {
         matrix[i][j] = (int)defaultState;
      }
   }
}

DFA::~DFA() {
   for (int i = 0; i < stateCount; i++) {
      delete[] matrix[i];
   }
   delete[] matrix;
}

void DFA::setTransition(int state, int transitionCharacter, int nextState) {
   matrix[(int)state][(int)transitionCharacter] = nextState;
}

void DFA::transition(int transitionCharacter) {
   if ((int)transitionCharacter >= transitionCharacterCount || (int)transitionCharacter < 0)
      throw std::out_of_range("Illegal transition character: "s + std::to_string((int)transitionCharacter));

   currentState = matrix[currentState][(int)transitionCharacter];
}

int DFA::getState() const {
   return currentState;
}

void DFA::setState(int state) {
   currentState = (int)state;
}
