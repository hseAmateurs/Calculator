//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <vector>
#include "token.h"
#include "mathExpr.h"
#include "funcHandler.h"
#include "shuntingYard.h"

// Подсчёт кол-ва знаков =
int countEqualSign(const MathExpr &expr) {
    int count = 0;
    for (const auto &ch: expr)
        if (ch == '=') count++;
    return count;
}

int main() {
    setbuf(stdout, 0);
    std::cout << "Введите пример:\n";
    // Класс обработки функций
    FuncHandler funcHandler;

    // Класс токенизации выражений
    MathExpr expression;

    // Флаг, отслеживающий, введено ли было главное выражение
    bool isMain = false;
    std::vector<Token> tokens;

    // Параметры вторичного выражения
    std::string funcName;
    int argsCount;
    std::vector<Token> funcTokens;
    // Цикл работает, пока таблица функций не будет полностью объявлена
    while (!isMain || !funcHandler.isFiled()) {
        getline(std::cin, expression);
        if (expression.empty()) {
            if (!isMain)
                std::cout << "Вы не ввели основное выражение\n";
            else
                funcHandler.printUndeclaredFunc();
            continue;
        }
        int countEqual = countEqualSign(expression);
        if (countEqual == 1) {
            std::tie(funcTokens, funcName, argsCount) = expression.tokenize(MathExpr::SECONDARY);
            // Факторизация необходима для отслеживания, необъявленных функций
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
    ShuntingYard shuntingYard(funcHandler);
    double res = shuntingYard.sumUp(tokens);
    std::cout << "Ответ:\n" <<  res - remainder(res, 0.0001) << "\n";
    return 0;
}
