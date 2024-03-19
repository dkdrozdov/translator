#pragma once

#include <fstream>
#include "common_includes.h"
#include "table.h"

namespace io {
   StaticTable readStaticTable(std::string path);
   MutableTable readMutableTable(std::string path);
   void writeStaticTable(std::string path, StaticTable& table);
   void writeMutableTable(std::string path, MutableTable& table);
}
