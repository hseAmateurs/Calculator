//
// Created by Shon on 17.01.2024.
//

#ifndef CALCULATOR_MATHEXPR_H
#define CALCULATOR_MATHEXPR_H

#include <string>
#include <vector>
#include <map>

#include "token.h"

class MathExpr : public std::string {
public:
    // Тип выражения: главное не имеет знака "=", тогда как остальные имеют
    enum Type {
        MAIN,
        SECONDARY
    };

    MathExpr() = default;

    explicit MathExpr(const std::string &str) : std::string(str) { };

    std::tuple<std::vector<Token>, std::string, int> tokenize(Type type);


private:

    char getOperatorPriority(const char &operatorName) const;

    Token::TokenType getCharType(const char &ch) const;

    // Обработка части после знака =
    std::vector<Token> handleDefinition(const std::string &expr);

    // Обработка части до знака =
    std::tuple<std::vector<Token>, std::string, int> handleDeclaration(const std::string &expr);

    bool isDeclarationValid(const std::vector<Token> &tokens) const;

    bool isOperator(const char &target) const;

    std::map<char, char> operatorPriority = {
            {'-', 1},
            {'+', 1},
            {'*', 2},
            {'/', 2},
            {'^', 3}
    };
};

#endif //CALCULATOR_MATHEXPR_H
