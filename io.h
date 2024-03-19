#pragma once

#include <fstream>
#include "common_includes.h"
#include "table.h"

namespace io {
   /*
      ������ ���� path � ������ �� ���� ����������� �������.
   */
   StaticTable readStaticTable(std::string path);

   /*
      ������ ���� path � ������ �� ���� ������������ �������.
   */
   MutableTable readMutableTable(std::string path);

   /*
      ���������� � ���� path ����������� �������.
   */
   void writeStaticTable(std::string path, StaticTable& table);

   /*
      ���������� � ���� path ������������ �������.
   */
   void writeMutableTable(std::string path, MutableTable& table);
}
