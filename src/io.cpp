#include "io.h"

namespace io {

   void logLexicalError(std::string message, int line, int column, std::ostream& errorStream)
   {
      errorStream << message << " at line " << line << "; character " << column << "." << std::endl;
   }

   void logError(std::string message, std::ostream& errorStream)
   {  
      errorStream << message << std::endl;
   }

   void logFileOpeningError(std::string name, std::ostream& errorStream)
   {
      errorStream << "Error: couldn't open file " << name << "." << std::endl;
   }

   StaticTable* readStaticTable(std::string path) {
      std::ifstream ifs(path);

      if (!ifs) {
         logFileOpeningError(path, std::cerr);
         return nullptr;
      }

      std::string token;
      std::vector<std::string> entries;

      while (ifs >> token) {
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
               throw std::exception(("Invalid escape character: "s + std::to_string(token[1])).c_str());
               break;
            }
            entries.push_back(escapeCharacter);
         }
         else {
            entries.push_back(token);
         }
      }
      ifs.close();

      return new StaticTable(entries);
   }

   enum readingState {
      SYMBOL_TYPE, DATA_TYPE, NAME, VALUE
   };

   // read file contents to StaticTable
   /*
   file format:
   [var int name value | lit int value]
   */
   MutableTable* readMutableTable(std::string path)
   {
      std::ifstream ifs(path);

      if (!ifs.is_open()) {
         logFileOpeningError(path, std::cerr);
         return nullptr;
      }

      std::string token;
      MutableTable* table = new MutableTable();

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

      while (ifs >> token) {
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
            else throw std::exception(("Invalid symbol type: "s + token).c_str());

            nextState = DATA_TYPE;

            break;
         }
         case DATA_TYPE:
         {
            if (token == "int") dataType = INT;
            else throw std::exception(("Invalid data type: "s + token).c_str());

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
               throw std::exception(("Invalid number: "s + token).c_str());
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
               table->add(Attributes::IntVariableAttributes(name, value));
            else
               table->add(Attributes::IntLiteralAttributes(value));
            shouldAddEntry = false;
         }

         state = nextState;
      }
      ifs.close();

      return table;
   }

   void writeStaticTable(std::string path, StaticTable& table) {
      std::ofstream ofs(path);

      if (!ofs)
      {
         logFileOpeningError(path, std::cerr);
         return;
      }

      std::vector<std::string> entries = table.table;
      auto n = entries.size();

      ofs << "index,name" << std::endl;


      for (int i = 0; i < n; i++) {
         std::string entry = entries[i];

         ofs << i << "\t";

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

      if (!ofs.is_open())
      {
         logFileOpeningError(path, std::cerr);
      }

      std::vector<TableEntry*> entries = table.table;
      auto n = entries.size();

      ofs << "data\tname\tvalue\tindex" << std::endl;

      for (int i = 0; i < n; i++) {
         TableEntry* entry = entries[i];
         if (entry == nullptr) continue;
         Attributes& attributes = entry->attributes;
         ofs <<
            attributes.dataType << "\t" <<
            attributes.name << "\t" <<
            attributes.value << "\t" <<
            entry->tableIndex <<
            std::endl;
      }

      ofs.close();
   }

   void writeTokens(std::string path, std::vector<Token> tokens)
   {
      std::ofstream ofs(path);

      if (!ofs.is_open())
      {
         logFileOpeningError(path, std::cerr); // TODO: replace std::cerr with stream stored in io class.
      }

      ofs << "line\tcolumn\tname\ttable\taddress" << std::endl;

      for (auto& token : tokens) {
         ofs << token.line << "\t" <<
            token.column << "\t" <<
            token.name << "\t" <<
            token.getType() << "\t" <<
            token.getAddress() << std::endl;
      }

      ofs.close();
   }

}