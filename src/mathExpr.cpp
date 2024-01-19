//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <stdexcept>

#include "MathExpr.h"

std::vector<Token> MathExpr::tokenize(MathExpr::Type type) {
    std::vector<Token> tokens;
    std::cout << *this << "\n";
    char priority = getOperatorPriority('^');
    return tokens;
}

char MathExpr::getOperatorPriority(const char &operatorName) const {
    char priority;
    try {
        priority = operatorPriority.at(operatorName);
    } catch (const std::out_of_range &ex) {
        std::cerr << "There's no operator: " << operatorName << "\n";
        exit(-1);
    }
    return priority;
}
