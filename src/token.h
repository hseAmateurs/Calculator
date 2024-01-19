//
// Created by Shon on 19.01.2024.
//

#ifndef CALCULATOR_TOKEN_H
#define CALCULATOR_TOKEN_H

#include <string>
#include <utility>

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

    Token(std::string value, TokenType tokenType, OperatorType operatorType = OperatorType::NONE);

private:
    union Value {
        std::string str;
        int int_num;
        double double_num;
    };

    Value value;
    const TokenType tokenType;
    const OperatorType operatorType;
};

#endif //CALCULATOR_TOKEN_H
