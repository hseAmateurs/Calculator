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
    void addFunc(std::string &name, int argsCount, std::vector<Token> tokens);

    // Добавляет фукнцию в таблицу, но не объявляет её
    void addFunc(std::string name);

    // Проверка, объявлены ли все фукнции в таблице
    bool isFiled() const;

    // Вычленение используемых фукнций и запись их в таблицу
    void factorizeFunc(const std::vector<Token> &tokens);

    bool isBuiltInFunc(const std::string &name) const;

    // Возвращает токены пользовательской фукнции
    // Если фукнция — встроенная, то вернёт её значение
    std::vector<Token> getFunc(const std::string &name, const std::vector<Token> &args);

    // Вывод необъявленных функций
    void raiseUndeclaredFunc() const;

    void printBuiltInFunc() const;

    static std::string toLower(const std::string &str);

    void clear();

private:
    // Возвращает кол-во аргументов фукнции
    // Если это перменная, то кол-во = 0
    int getArgsCount(const std::string &name) const;

    // Вычисления встроенных функций
    double funcCalc(const std::string &funcName, double x) const;

    const double accuracy = 1e-5;

    std::map<std::string, std::pair<int, std::vector<Token>>> functions;

    std::vector<std::string> builtInFunctions = {"pi", "e",
                                                 "abs", "sqrt", "ln", "lg", "log2", "sign", "exp",
                                                 "sin", "cos", "tg", "ctg", "arcsin", "arccos", "arctg", "arcctg"};
};


#endif //CALCULATOR_FUNCHANDLER_H
