//
// Created by Shon on 26.01.2024.
//

#ifndef CALCULATOR_FUNCHANDLER_H
#define CALCULATOR_FUNCHANDLER_H

#include <map>
#include <string>
#include <vector>

#include "token.h"

class FuncHandler {
public:
    // Непосрдественное добавление фукнции в таблицу
    bool addFunc(std::string &name, int argsCount, std::vector<Token> tokens);

    // Добавляет фукнцию в таблицу, но не объявляет её
    bool addFunc(std::string name);

    // Проверка, объявлены ли все фукнции в таблице
    bool isFiled() const;

    // Вычленение используемых фукнций и запись их в таблицу
    void factorizeFunc(const std::vector<Token> &tokens);

    // Возвращает кол-во аргументов фукнции
    // Если это перменная, то кол-во = 0
    int getArgsCount(std::string &name) const;

    bool isBuiltInFunc(const std::string &name) const;

    // Возвращает токены пользовательской фукнции
    // Если фукнция — встроенная, то вернёт её значение
    std::vector<Token> getFunc(const std::string &name, const std::vector<Token> &args);

    // Вывод необъявленных функций
    void printUndeclaredFunc() const;

    // Вычисления арифметических действий
    Token calculate(const Token &operToken, const Token &aToken, const Token &bToken) const;

private:
    // Вычисления встроенных функций
    double funcCalc(const std::string &funcName, double x) const;

    std::string toLower(const std::string &str) const;

    bool error(const std::string &msg) const;

    std::map<std::string, std::pair<int, std::vector<Token>>> functions;

    // Счётчик необъявленных функций
    int emptyRows = 0;

    std::vector<std::string> builtInFunctions = {"pi", "e",
                                                 "abs", "sqrt", "ln", "lg", "log2", "sign", "exp",
                                                 "sin", "cos", "tg", "ctg", "arcsin", "arccos", "arctg", "arcctg"};
};


#endif //CALCULATOR_FUNCHANDLER_H