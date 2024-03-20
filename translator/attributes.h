#pragma once

#include "common_includes.h"

/*
  ���������� ���� ��������.
*/
enum SymbolType
{
   LITERAL,    // ��� ���������.
   IDENTIFIER, // ��� ��������������.
};

/*
  ���������� ���� ������.
*/
enum DataType {
   INT,        // ��� ������������� ������.
};

/*
   ���������� ���������� ����� ��������� ������ ������� ��������.
*/
struct Attributes {
public:
   SymbolType symbolType; 
   DataType dataType;
   std::string name;       // ��� �������. ��� ������������� �������� ������� ��� %d � ��������� �������� ���������.
   int value;              // ������������� �������� �������.
   int* address;           // ����� ������� ������, ���������� ��� �������� ��������.
   int memory;             // ������ ���������� ������� ������.

   Attributes(SymbolType _symbolType, DataType _dataType, std::string _name, int _value);

   /*
      ������ ��������� ��� �������������� ������������� ����������.
   */
   static Attributes IntVariableAttributes(std::string name, int value);

   /*
      ������ ��������� ��� �������������� ������������� ���������.
   */
   static Attributes IntLiteralAttributes(int value);

   /*
      ������� � ������� ��������.
   */
   void print() const;
};
