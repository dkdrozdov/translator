#include "io.h"

namespace io {
   StaticTable readStaticTable(std::string path) {
      std::ifstream operatorsfs(path);

      if (!operatorsfs) {
         throw std::exception(("Error opening file: " + path).c_str());
      }

      std::string token;
      std::vector<std::string> entries;

      while (operatorsfs >> token) {
         if (token.size() == 2 && token[0] == '\\') {
            std::string escapeCharacter = "";

            switch (token[1])
            {
            case 'n': {
               escapeCharacter = "\n";
               break;
            }
            case 's': {
               escapeCharacter = " ";
               break;
            }
            case 't': {
               escapeCharacter = "\t";
               break;
            }
            case '\\': {
               escapeCharacter = "\\";
               break;
            }
            default:
               throw std::exception("Invalid escape character: " + token[1]);
               break;
            }
            entries.push_back(escapeCharacter);
         }
         else {
            entries.push_back(token);
         }
      }
      operatorsfs.close();

      return StaticTable(entries);
   }

   enum readingState {
      SYMBOL_TYPE, DATA_TYPE, NAME, VALUE
   };

   // read file contents to StaticTable
   /*
   file format:
   [var int name value | lit int value]
   */
   MutableTable readMutableTable(std::string path)
   {
      std::ifstream operatorsfs(path);
      std::string token;
      MutableTable table;

      // Finite automata for reading
      readingState state = SYMBOL_TYPE;
      readingState nextState;
      bool isIdentifier = false;
      bool shouldAddEntry = false;

      // Entry data
      SymbolType symbolType;
      DataType dataType;
      std::string name{};
      int value;

      while (operatorsfs >> token) {
         switch (state)
         {
         case SYMBOL_TYPE:
         {
            if (token == "var")
            {
               symbolType = IDENTIFIER;
               isIdentifier = true;
            }
            else if (token == "lit")
            {
               symbolType = LITERAL;
               isIdentifier = false;
            }
            else throw std::exception(("Invalid symbol type: " + token).c_str());

            nextState = DATA_TYPE;

            break;
         }
         case DATA_TYPE:
         {
            if (token == "int") dataType = INT;
            else throw std::exception(("Invalid data type: " + token).c_str());

            nextState = isIdentifier ? NAME : VALUE;

            break;
         }
         case NAME:
         {
            name = token;

            nextState = VALUE;

            break;
         }
         case VALUE:
         {
            try
            {
               value = std::stoi(token);
            }
            catch (...)
            {
               throw std::exception(("Invalid number: " + token).c_str());
            }

            nextState = SYMBOL_TYPE;
            shouldAddEntry = true;

            break;
         }
         default:
            throw std::exception("Invalid reading state.");
         }

         if (shouldAddEntry)
         {
            if (isIdentifier)
               table.add(Attributes::IntVariableAttributes(name, value));
            else
               table.add(Attributes::IntLiteralAttributes(value));
            shouldAddEntry = false;
         }

         state = nextState;
      }
      operatorsfs.close();

      return table;
   }

   void writeStaticTable(std::string path, StaticTable& table) {
      std::ofstream ofs(path);

      if (!ofs) throw std::exception(("Error opening file: " + path).c_str());

      std::vector<std::string> entries = table.table;
      int n = entries.size();

      for (int i = 0; i < n; i++) {
         std::string entry = entries[i];

         if (entry == "\t") {
            ofs << "\\t" << std::endl;
         }
         else if (entry == " ") {
            ofs << "\\s" << std::endl;
         }
         else if (entry == "\n") {
            ofs << "\\n" << std::endl;
         }
         else
            ofs << entries[i] << std::endl;
      }

      ofs.close();
   }

   void writeMutableTable(std::string path, MutableTable& table) {
      std::ofstream ofs(path);

      if (!ofs) throw std::exception(("Error opening file: " + path).c_str());

      std::vector<TableEntry*> entries = table.table;
      int n = entries.size();


      for (int i = 0; i < n; i++) {
         TableEntry* entry = entries[i];
         if (entry == nullptr) continue;
         Attributes& attributes = entry->attributes;
         ofs <<
            attributes.symbolType << " " <<
            attributes.dataType << " " <<
            attributes.name << " " <<
            attributes.value.value <<
            std::endl;
      }

      ofs.close();
   }
}
