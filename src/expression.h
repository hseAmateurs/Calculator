//
// Created by Shon on 17.01.2024.
//

#ifndef CALCULATOR_TOKENIZER_H
#define CALCULATOR_TOKENIZER_H

#include <string>
#include <vector>
#include <map>
#include "token.h"

class Tokenizer {
public:
    enum ExpressionType {
        MAIN,
        SECONDARY
    };

    static void tokenize(const std::string &expression, std::vector<Token> &tokens, ExpressionType type);

private:
    static char getTokenPriority(Token::TokenType tokenType, char operatorName = 0);
    static std::map<char, char> operatorPriority;
};

std::map<char, char> Tokenizer::operatorPriority = {
        {'-', 1},
        {'+', 1},
        {'*', 2},
        {'/', 2},
        {'^', 3}
};


#endif //CALCULATOR_TOKENIZER_H
