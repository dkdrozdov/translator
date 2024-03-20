#include "token.h"

Token::Token(TokenType _type, int _address, int _line, int _column, std::string _name, Tables* _tables) :
   type(_type), address(_address), line(_line), column(_column), name(_name), tables(_tables) {}
