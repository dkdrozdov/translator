#include "dfa.h"

DFA::DFA(int _stateCount, int _transitionCharacterCount, int failState) : stateCount(_stateCount), transitionCharacterCount(_transitionCharacterCount) {
   matrix = new int* [_stateCount];

   for (int i = 0; i < stateCount; i++) {
      matrix[i] = new int[_transitionCharacterCount];
      for (int j = 0; j < transitionCharacterCount; j++) {
         matrix[i][j] = failState;
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
   matrix[state][transitionCharacter] = nextState;
}

void DFA::transition(int transitionCharacter) {
   if (transitionCharacter >= transitionCharacterCount)
      throw std::out_of_range("Illegal transition character: "s + std::to_string(transitionCharacter));

   currentState = matrix[currentState][transitionCharacter];
}

int DFA::getState() const {
   return currentState;
}

void DFA::setState(int state) {
   currentState = state;
}
