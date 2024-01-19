//
// Created by Shon on 19.01.2024.
//

#include "token.h"

Token::Token(std::string value, Token::TokenType tokenType, Token::OperatorType operatorType)
        : tokenType(tokenType), operatorType(operatorType) {
    switch (tokenType) {
        case INT:
            this->value.int_num = std::stoi(value);
            break;
        case DOUBLE:
            this->value.double_num = std::stod(value);
            break;
        default:
            this->value.str = std::move(value);
    }
}
