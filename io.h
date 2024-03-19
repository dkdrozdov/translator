#pragma once

#include <fstream>
#include "common_includes.h"
#include "table.h"

namespace io {
   /*
      Читает файл path и строит по нему статическую таблицу.
   */
   StaticTable readStaticTable(std::string path);

   /*
      Читает файл path и строит по нему динамическую таблицу.
   */
   MutableTable readMutableTable(std::string path);

   /*
      Записывает в файл path статическую таблицу.
   */
   void writeStaticTable(std::string path, StaticTable& table);

   /*
      Записывает в файл path динамическую таблицу.
   */
   void writeMutableTable(std::string path, MutableTable& table);
}
