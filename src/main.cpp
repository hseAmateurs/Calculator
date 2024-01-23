//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <vector>
#include "token.h"
#include "mathExpr.h"

int main() {
    setbuf(stdout, 0);
    MathExpr expression("sin(5*(x) + 9) - 22/984^(f(xm, y+(90-8)))");
    std::vector<Token> tokens;
    tokens = expression.tokenize(MathExpr::Type::MAIN);
    for (int i = 0; i < tokens.size(); ++i) {
        std::cout << tokens[i].value << " "
                  << tokens[i].tokenType << " "
                  << tokens[i].operatorType << " "
                  << (int)tokens[i].operatorPriority << std::endl;
    }
    std::cout << "end\n";
    return 0;
}
