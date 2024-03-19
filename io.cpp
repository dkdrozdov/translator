//#include "io.h"
//
//enum StaticTableEntryReadingState
//{
//   START, LEXEME, ATTRIBUTE_TYPE, ATTRIBUTE_VALUE, END
//};
//
//StaticTable readIntoStaticTable(std::string path) {
//   std::ifstream operatorsfs(path);
//   // read file contents to StaticTable
//   /*
//   file format:
//   lexeme <lexeme> [<attribute type> <attribute value>]
//   */
//
//   std::string token;
//   std::vector<TableEntry> entries;
//   std::vector<Attribute*> attributes;
//   StaticTableEntryReadingState state = START;
//   StaticTableEntryReadingState nextState = START;
//   Lexeme* lexeme = nullptr;
//   Attribute* attribute = nullptr;
//   AttributeType attributeType;
//
//   bool addEntry = false;
//
//   while (operatorsfs >> token) {
//      if (state == START && token == "lexeme") {
//         nextState = LEXEME;
//      }
//
//      if (state == LEXEME) {
//         // Check for duplicates
//         for (TableEntry entry : entries) {
//            if (entry.getLexeme() == token)
//               throw std::exception(("Duplicate lexeme: " + token).c_str());
//         }
//
//         lexeme = new Lexeme(token);
//         nextState = ATTRIBUTE_TYPE;
//      }
//
//      if (state == ATTRIBUTE_TYPE) {
//         if (token == "lexeme") {
//            addEntry = true;
//            nextState = LEXEME;
//         }
//         else {
//            attributeType = Attribute::parseType(token);
//            nextState = ATTRIBUTE_VALUE;
//         }
//      }
//
//      if (state == ATTRIBUTE_VALUE) {
//         switch (attributeType)
//         {
//         case LANGUAGE_UNIT_TYPE:
//         {
//            attribute = new LanguageUnitAttribute(LanguageUnitAttribute::parseType(token));
//            break;
//         }
//         case VALUE: {
//            try {
//               int value = std::stoi(token);
//               attribute = new ValueAttribute(value);
//            }
//            catch (...) {
//               throw std::exception(("Invalid value: " + token).c_str());
//            }
//
//            break;
//         }
//         default:
//         {
//            throw std::exception(("Invalid attribute type: " + token).c_str());
//
//            break;
//         }
//         }
//         nextState = ATTRIBUTE_TYPE;
//         attributes.push_back(attribute);
//      }
//
//      if (addEntry || operatorsfs.peek() == EOF) {
//         if (lexeme != nullptr) {
//
//            entries.push_back(TableEntry(*lexeme, Attributes(attributes)));
//
//            delete lexeme;
//            lexeme = nullptr;
//
//            attributes = {}; // Удалять аттрибуты не нужно, поскольку на них ссылаются табличные записи (поле attributes в TableEntry).
//         }
//
//         addEntry = false;
//      }
//
//      state = nextState;
//   }
//   operatorsfs.close();
//
//   if (!(state == START || state == ATTRIBUTE_TYPE)) {
//      throw std::exception(("Unexpected end of file"));
//   }
//
//   return StaticTable(entries);
//}
