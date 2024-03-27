#include "token.h"


 TokenType Token::getType() const { return type; }

 bool Token::isInMutableTable() const {
    return type == IDENTIFIER_TOKEN_TYPE || type == LITERAL_TOKEN_TYPE;
}

 int Token::getAddress() {
    if (!isInMutableTable()) return address;
    try
    {
        switch (type)
        {
        case IDENTIFIER_TOKEN_TYPE:
        {
            return tables->identifiers->find(name)->tableIndex;
            break;
        }
        case LITERAL_TOKEN_TYPE:
        {
            return tables->literals->find(name)->tableIndex;

            break;
        }
        }
    }
    catch (...)
    {
        throw std::exception("Error: couldn't get token address.");
    }
    throw std::exception("Error: couldn't get token address.");
}
