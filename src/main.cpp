//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <vector>
#include "token.h"
#include "mathExpr.h"
#include "funcHandler.h"

int countEqualSign(const MathExpr &expr) {
    int count = 0;
    for (const auto &ch : expr)
        if (ch == '=') count++;
    return count;
}
/*
f(x, y) = x + a + y
y1 = f(1, a) + 8 * (7 - 7)
f(tr(1, 2), 1) + a + y1
 */
int main() {
    setbuf(stdout, 0);
    FuncHandler funcHandler;
    MathExpr expression;
    bool isMain = false;
    std::vector<Token> tokens;
    while(!isMain || !funcHandler.isFiled()) {
        getline(std::cin, expression);
        int countEqual = countEqualSign(expression);
        if (countEqual == 1) {
            std::string funcName;
            int argsCount;
            std::vector<Token> funcTokens;
            std::tie(funcTokens, funcName, argsCount) = expression.tokenize(MathExpr::SECONDARY);
            funcHandler.factorizeFunc(funcTokens);
            funcHandler.addFunc(funcName, argsCount, funcTokens);
        }
        else if (countEqual == 0) {
            if (isMain) {
                std::cerr << "Вы уже вводили главное выражение";
                exit(-1);
            }
            tokens = std::get<0>(expression.tokenize(MathExpr::MAIN));
            funcHandler.factorizeFunc(tokens);
            isMain = true;
        }
        else {
            std::cerr << "Синтаксическая ошибка";
            exit(-1);
        }
    }

//    for (const auto & token : tokens) {
//        std::cout << token.value << " "
//                  << token.tokenType << " "
//                  << token.operatorType << " "
//                  << (int)token.operatorPriority << std::endl;
//    }
    std::cout << "end\n";
    return 0;
}
