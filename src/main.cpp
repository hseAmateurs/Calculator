//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <vector>
#include "token.h"
#include "mathExpr.h"

int countEqualSign(const MathExpr &expr) {
    int count = 0;
    for (const auto &ch : expr)
        if (ch == '=') count++;
    return count;
}

int main() {
    setbuf(stdout, 0);
    MathExpr expression("f(x, y, z) = sin(5*(x) + 9) - 22/984^(f(xm, y+(90-8)))");
    bool isMain = false;
    std::vector<Token> tokens;
    for (int i = 0; i < 3; ++i) {
        getline(std::cin, expression);
        int countEqual = countEqualSign(expression);
        if (countEqual == 1)
            expression.tokenize(MathExpr::SECONDARY);
        else if (countEqual == 0) {
            if (isMain) {
                std::cerr << "Вы уже вводили главное выражение";
                exit(-1);
            }
            tokens = expression.tokenize(MathExpr::MAIN);
            isMain = true;
        }
        else {
            std::cerr << "Синтаксическая ошибка";
            exit(-1);
        }
    }

    for (const auto & token : tokens) {
        std::cout << token.value << " "
                  << token.tokenType << " "
                  << token.operatorType << " "
                  << (int)token.operatorPriority << std::endl;
    }
    std::cout << "end\n";
    return 0;
}
