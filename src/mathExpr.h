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

    MathExpr();

    explicit MathExpr(const std::string &str) : std::string(str) { std::cerr << "Init\n"; };

    std::vector<Token> tokenize(Type type);

private:
    char getOperatorPriority(const char &operatorName) const;

    void handleMainExpr();

    void handleSecondaryExpr(std::vector<Token> &tokens);

    bool isOperator(const char& target) const;

    void error(const std::string& msg) const;

    std::vector<Token> tokens;

    std::map<char, char> operatorPriority = {
            {'-', 1},
            {'+', 1},
            {'*', 2},
            {'/', 2},
            {'^', 3}
    };

    std::vector<char> operators;
};

#endif //CALCULATOR_MATHEXPR_H
