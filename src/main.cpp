//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <vector>
#include <limits>
#include <cmath>
#include "token.h"
#include "mathExpr.h"
#include "funcHandler.h"
#include "shuntingYard.h"
#include "calcException.h"
#include "tokenizeException.h"

// Подсчёт кол-ва знаков =
int countEqualSign(const MathExpr &expr) {
    int count = 0;
    for (const auto &ch: expr)
        if (ch == '=') count++;
    return count;
}

int main() {
    setbuf(stdout, 0);
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

    bool clearInput = true;
    bool isRun = true;
    while (isRun) {
        if(clearInput) std::cout << "Введите пример:\n";
        else std::cout << "Продолжите ввод:\n";
        clearInput = true;
        try {
            // Цикл работает, пока таблица функций не будет полностью объявлена
            while (!isMain || !funcHandler.isFiled()) {
                getline(std::cin, expression);
                if (expression.empty()) {
                    if (!isMain)
                        throw TokenizeException(TokenizeException::NO_MAIN);
                    else {
                        funcHandler.printUndeclaredFunc();
                        std::cout << "Продолжите ввод:\n";
                    }
                    continue;
                }
                if(FuncHandler::toLower(expression) == "exit") {
                    isRun = false;
                    break;
                }
                int countEqual = countEqualSign(expression);
                if (countEqual == 1) {
                    std::tie(funcTokens, funcName, argsCount) = expression.tokenize(MathExpr::SECONDARY);
                    // Факторизация необходима для отслеживания, необъявленных функций
                    funcHandler.factorizeFunc(funcTokens);
                    funcHandler.addFunc(funcName, argsCount, funcTokens);
                }
                else if (countEqual == 0) {
                    if (isMain)
                        throw TokenizeException(TokenizeException::DUBLICATE_MAIN);
                    tokens = std::get<0>(expression.tokenize(MathExpr::MAIN));
                    funcHandler.factorizeFunc(tokens);
                    isMain = true;
                }
                else {
                    throw TokenizeException(TokenizeException::SYNTAX_ERROR);
                }
            }
            if(!isRun) continue;
            ShuntingYard shuntingYard(funcHandler);
            double res = shuntingYard.sumUp(tokens);
            std::cout << "Ответ:\n" <<  res - remainder(res, 0.0001) << "\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        catch (const TokenizeException &ex) {
            std::cerr << "Ошибка: " << ex.desc() << "\n";
            if(!ex.message.empty())
                std::cerr << "< " << ex.message << " >\n";
            std::cerr.flush();
            clearInput = false;
        }
        catch (const CalcException &ex) {
            std::cerr << "Ошибка: " << ex.desc() << "\n";
            if(!ex.message.empty())
                std::cerr << "< " << ex.message << " >\n";
            std::cerr << std::endl;
            std::cerr.flush();
        }
        if(isRun && clearInput) {
            isMain = false;
            funcHandler.clear();
        }
    }
    return 0;
}
