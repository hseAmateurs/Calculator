//
// Created by Shon on 17.01.2024.
//

#include "tokenizer.h"

void Tokenizer::tokenize(const std::string &expression, std::vector<Token> &tokens, ExpressionType type) {
    getTokenPriority(Token::DOUBLE);
}

char Tokenizer::getTokenPriority(Token::TokenType tokenType, char operatorName) {
    char priority = -1;
    switch (tokenType) {
        case Token::OPERATOR:
            priority = operatorPriority.at(operatorName);
            break;
        case Token::SEPARATOR:
            break;
        case Token::VAR: case Token::FUNC:
            break;
        default:
            break;
    }
    return priority;
}
