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
    enum Type {
        MAIN,
        SECONDARY
    };

    MathExpr() : std::string() { };

    explicit MathExpr(const std::string &str) : std::string(str) { };

    std::vector<Token> tokenize(Type type);

private:
    char getOperatorPriority(const char &operatorName) const;

    std::map<char, char> operatorPriority = {
            {'-', 1},
            {'+', 1},
            {'*', 2},
            {'/', 2},
            {'^', 3}
    };
};

#endif //CALCULATOR_MATHEXPR_H
