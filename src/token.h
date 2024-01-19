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
        INT,
        DOUBLE,
        OPERATOR,
        SEPARATOR,
        L_PARANTHESIS,
        R_PARANTHESIS,
        VAR,
        FUNC
    };

    enum OperatorType {
        NONE,
        UNARY,
        BINARY
    };

    const std::string value;
    const TokenType tokenType;
    const char operatorPriority;
    const OperatorType operatorType;

    Token(std::string value, TokenType tokenType, char operatorPriority = -1, OperatorType operatorType = OperatorType::NONE)
            : value(std::move(value)),
              tokenType(tokenType),
              operatorPriority(operatorPriority),
              operatorType(operatorType) { };
};

#endif //CALCULATOR_TOKEN_H
