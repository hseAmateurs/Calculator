//
// Created by Shon on 17.01.2024.
//

#ifdef _WIN32
#include <Windows.h>
#endif

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
//    setbuf(stdout, 0);
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    std::cout << "Добро пожаловать в калькулятор!\n"
                 "Введите \x1B[35mhelp\033[0m, чтобы посмотреть список встроенных функций.\n"
                 "Введите \x1B[31mexit\033[0m, чтобы выйти.\n\n";

    // Класс обработки функций
    FuncHandler funcHandler;
    ShuntingYard shuntingYard(&funcHandler);

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
        if (clearInput) std::cout << "\x1B[32mВведите пример:\033[0m\n";
        else std::cout << "\x1B[33mПродолжите ввод:\033[0m\n";
        clearInput = true;
        try {
            // Цикл работает, пока таблица функций не будет полностью объявлена
            while (!isMain || !funcHandler.isFiled()) {
                getline(std::cin, expression);
                if (expression.empty()) {
                    if (!isMain) throw TokenizeException(TokenizeException::NO_MAIN);
                    else funcHandler.raiseUndeclaredFunc();
                }
                if (FuncHandler::toLower(expression) == "exit") {
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
                    if (FuncHandler::toLower(expression) == "help") {
                        funcHandler.printBuiltInFunc();
                        tokens.clear();
                        clearInput = false;
                        break;
                    }
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
            if (!isRun) continue;
            if (tokens.empty()) continue;
            double res = shuntingYard.sumUp(tokens);
            std::cout << "Ответ:\n" << res - remainder(res, 0.0001) << "\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        catch (const CalcException &ex) {
            std::cout << "\n\x1B[31mОшибка: " << ex.desc() << "\n";
            if (!ex.message.empty())
                std::cout << "< " << ex.message << " >\n";
            std::cout << "\033[0m\n";

            if (typeid(ex) == typeid(TokenizeException))
                clearInput = false;
        }
        if (isRun && clearInput) {
            isMain = false;
            funcHandler.clear();
            shuntingYard.clear();
            tokens.clear();
        }
    }
    return 0;
}
