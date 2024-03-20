#pragma once

#include "common_includes.h"
#include "table.h"

/*
   ��������� ���� �������.
   ������ ��� ������������� ����� ������� ��������.
*/
enum TokenType {
   KEYWORD_TOKEN_TYPE,     // ������������� ������� �������� ����.
   OPERATOR_TOKEN_TYPE,    // ������������� ������� ����������.
   SEPARATOR_TOKEN_TYPE,   // ������������� ������� ������������.
   IDENTIFIER_TOKEN_TYPE,  // ������������� ������� ���������������.
   LITERAL_TOKEN_TYPE,     // ������������� ������� ��������.
   BRACKET_TOKEN_TYPE,     // ������������� ������� ������.
};

/*
   ��������� ����� �������.
   ������� �� ������ ������� � ��������������, ����������� �� �������������� �������.
   �������� ��������������� ���� ��� ��������� ������������.
*/
struct Token {
private:
   TokenType type;

   Tables* tables; // ������ �� ������� (����������� ���� ��� ���������� �������, ��������� ������� ��� ���).
   int address;   // ������ � ������� (������ ��� ����������� ������)

public:

   TokenType getType() const;


   /*
      ���������� ������, ���� ��������� �� ������������ �������.
   */
   bool isInMutableTable() const;

   /*
      ����������� ����, ��������� ������� ����� �������������� � �������� ����������� (��� ������������ ������)
   */
   int getAddress();;

   // ��������������� ����:
   int line;         // ����� ������ � �������� ����.
   int column;       // ����� ������� � �������� ����.
   std::string name; // ��� �������.

   Token(TokenType _type, int _address, int _line, int _column, std::string _name, Tables* _tables) :
      type(_type), address(_address), line(_line), column(_column), name(_name), tables(_tables) {}
};