//
// Created by Shon on 19.01.2024.
//

#ifndef CALCULATOR_TOKEN_H
#define CALCULATOR_TOKEN_H

#include <string>
#include <map>

class Token {
public:
    enum TokenType {
        UNDEFINED,
        INT,
        DOUBLE,
        OPERATOR,
        SEPARATOR,
        L_PARANTHESIS,
        R_PARANTHESIS,
        VAR,
        FUNC,
        ARG
    };

    enum OperatorType {
        NONE,
        UNARY,
        BINARY
    };

    std::string value;
    TokenType tokenType;
    char operatorPriority;
    OperatorType operatorType;

    Token(std::string value, TokenType tokenType, char operatorPriority = -1,
          OperatorType operatorType = OperatorType::NONE) :
            value(std::move(value)),
            tokenType(tokenType),
            operatorType(operatorType),
            operatorPriority(operatorPriority) { };
};

#endif //CALCULATOR_TOKEN_H