//
// Created by Shon on 26.01.2024.
//

#include "funcHandler.h"

#include <iostream>
#include <valarray>

bool FuncHandler::addFunc(std::string &name, const int argsCount, const std::vector<Token> tokens) {
    // Если функция уже занесена в список
    std::cout << "addFunc: " << name << " " << argsCount << " " << tokens.size() << "\n";
    if (functions.find(name) != functions.end()) {
        emptyRows--;
        // Скипаем т.к. пустая фукнция уже есть в словаре
        if (tokens.empty()) return true;
        // И она объявлена
        if (!functions.at(name).second.empty()) {
            if (tokens.empty()) return true;
            else return error("Функция/переменная уже объявлена");
        }
    }
    functions[std::move(name)] = std::make_pair(argsCount, tokens);
    std::cout << "Current map: " << emptyRows << "\n";
    for (const auto &el: functions) {
        std::cout << el.first << " " << el.second.first << " " << el.second.second.size() << "\n";
    }
    return true;
}

bool FuncHandler::addFunc(std::string name) {
    emptyRows++;
    return addFunc(name, 0, {});
}

bool FuncHandler::error(const std::string &msg) const {
    std::cerr << msg << std::endl;
    exit(-1);
    return false;
}

bool FuncHandler::isFiled() const {
    return !emptyRows;
}

void FuncHandler::factorizeFunc(const std::vector<Token> &tokens) {
    for (const auto &token: tokens) {
        if (token.tokenType != Token::VAR && token.tokenType != Token::FUNC) continue;
        addFunc(token.value);
    }
}

bool FuncHandler::isFuncValid(std::string &name, int argsCount) const {
    if (functions.find(name) == functions.end())
        return error("Функция не объявлена");
    return (functions.at(name).first == argsCount);
}

std::vector<Token> FuncHandler::getFunc(std::string &name, std::vector<Token> &args) {
    if (functions.find(name) == functions.end())
        error("Функция не объявлена");
    std::vector<Token> tokens = functions.at(name).second;

    size_t argsSize = args.size();
    for (int i = 0; i < argsSize; ++i) {
        for (auto &token: tokens) {
            if (token.tokenType == Token::ARG && token.operatorPriority == i)
                token = args[i];
        }
    }
    return tokens;
}

void FuncHandler::printUndeclaredFunc() const {
    std::cout << "Вы не объявили функции/переменные:\n";
    bool first = true;
    for (const auto &el: functions) {
        if (!el.second.second.empty()) continue;
        if (first) {
            std::cout << el.first;
            first = false;
        }
        else std::cout << ", " << el.first;
    }
    std::cout << std::endl;
}

Token FuncHandler::calculate(const Token &operToken, const Token &aToken, const Token &bToken) const {
    double ans;
    double a = std::stod(aToken.value);
    double b = std::stod(bToken.value);

    if(operToken.tokenType == Token::OPERATOR)
        ans = arithmeticCalc(operToken.value[0], a, b);
    else
        ans = funcCalc(operToken.value, a, b);

    Token res;
    if (aToken.tokenType == Token::DOUBLE || bToken.tokenType == Token::DOUBLE) {
        res.tokenType = Token::DOUBLE;
        res.value = std::to_string(ans);
    }
    else {
        res.tokenType = Token::INT;
        res.value = std::to_string((int)ans);
    }
    return res;
}

double FuncHandler::arithmeticCalc(const char oper, const double &a, const double &b) const {
    double res;
    switch (oper) {
        case '+':
            res = a + b;
            break;
        case '-':
            res = a - b;
            break;
        case '*':
            res = a * b;
            break;
        case '/':
            res = a / b;
            break;
        case '^':
            res = pow(a, b);
            break;
        default:
            error("Неизвестный оператор");
    }
    return res;
}

double FuncHandler::funcCalc(std::string funcName, const double &a, const double &b) const {
    toLower(funcName);

    double res;
    if(funcName == "abs") {

    }
    else if(funcName == "sqrt") {

    }
    else if(funcName == "ln") {

    }
    else if(funcName == "lg") {

    }
    else if(funcName == "log2") {

    }
    else if(funcName == "sign") {

    }
    else if(funcName == "exp") {

    }
    else if(funcName == "sin") {

    }
    else if(funcName == "cos") {

    }
    else if(funcName == "tn") {

    }
    else if(funcName == "ctg") {

    }
    else if(funcName == "arcsin") {

    }
    else if(funcName == "arccos") {

    }
    else if(funcName == "arctg") {

    }
    else if(funcName == "arcctg") {

    }
    else error("Неизвестная функция");
    return res;
}

void FuncHandler::toLower(std::string& str) const {
    for (char& c : str)
        c = std::tolower(c);
}
