//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <vector>
#include "token.h"
#include "mathExpr.h"

int main() {
    MathExpr expression("8 - 9 / 2*(sin(x))");
//    getline(std::cin, expression);
    std::vector<Token> tokens;
    tokens = expression.tokenize(MathExpr::Type::MAIN);
    tokens.emplace_back("a", Token::VAR, 4);
    std::cout << "end\n";
    return 0;
}
