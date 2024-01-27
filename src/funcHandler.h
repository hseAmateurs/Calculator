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
    bool addFunc(std::string &name, int argsCount, std::vector<Token> tokens);

    bool addFunc(std::string name);

    bool isFiled() const;

    void factorizeFunc(const std::vector<Token> &tokens);

    int getArgsCount(std::string &name) const;

    bool isBuiltInFunc(const std::string &name) const;

    std::vector<Token> getFunc(const std::string &name, const std::vector<Token> &args);

    void printUndeclaredFunc() const;

    Token calculate(const Token &operToken, const Token &aToken, const Token &bToken) const;

private:
    double funcCalc(const std::string &funcName, double x) const;

    std::string toLower(const std::string &str) const;

    bool error(const std::string &msg) const;

    std::map<std::string, std::pair<int, std::vector<Token>>> functions;

    int emptyRows = 0;

    std::vector<std::string> builtInFunctions = {"pi", "e",
                                                 "abs", "sqrt", "ln", "lg", "log2", "sign", "exp",
                                                 "sin", "cos", "tg", "ctg", "arcsin", "arccos", "arctg", "arcctg"};
};


#endif //CALCULATOR_FUNCHANDLER_H
