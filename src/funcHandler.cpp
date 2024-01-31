//
// Created by Shon on 26.01.2024.
//

#include "funcHandler.h"

#include <iostream>
#include <valarray>
#include <cmath>
#include "calcException.h"
#include "tokenizeException.h"

void FuncHandler::addFunc(std::string &name, const int argsCount, const std::vector<Token> tokens) {
    // Дебаг информация
#ifdef DEBUG
    std::cout << "addFunc: " << name << " " << argsCount << " " << tokens.size() << "\n";
#endif
    if (isBuiltInFunc(name)) {
        if (tokens.empty()) return;
        else
            throw TokenizeException(TokenizeException::BUILTIN_REDECLARATION, name);
    }

    // Если функция уже занесена в список
    if (functions.find(name) != functions.end()) {
        // Скипаем т.к. пустая фукнция уже есть в таблице
        if (tokens.empty()) return;
        // И она объявлена
        if (!functions.at(name).second.empty())
            throw TokenizeException(TokenizeException::ALREADY_DECLARED, name);
    }
    functions[std::move(name)] = std::make_pair(argsCount, tokens);
    // Дебаг информация
#ifdef DEBUG
    //    std::cout << "Current map: " << emptyRows << "\n";
        for (const auto &el: functions) {
            std::cout << el.first << " " << el.second.first << " " << el.second.second.size() << "\n";
        }
        std::cout << std::endl;
#endif
}

void FuncHandler::addFunc(std::string name) {
    addFunc(name, 0, {});
}

bool FuncHandler::isFiled() const {
    for (const auto &el: functions)
        if (el.second.second.empty()) return false;
    return true;
}

void FuncHandler::factorizeFunc(const std::vector<Token> &tokens) {
    for (const auto &token: tokens) {
        // Если встречаем FUNC/VAR, то вносим в таблицу
        if (token.tokenType != Token::VAR && token.tokenType != Token::FUNC) continue;
        addFunc(token.value);
    }
}

int FuncHandler::getArgsCount(const std::string &name) const {
    if (isBuiltInFunc(name)) {
        std::string lowerName = toLower(name);
        if (lowerName == "e" || lowerName == "pi") return 0;
        else return 1;
    }
    if (functions.find(name) == functions.end())
        throw CalcException(CalcException::UNDECLARED_FUNC, name);

    return functions.at(name).first;
}

std::vector<Token> FuncHandler::getFunc(const std::string &name, const std::vector<Token> &args) {
    size_t argsSize = args.size();

    // Проверка количества подаваемых аргументов с необходимым
    if (argsSize != getArgsCount(name))
        throw CalcException(CalcException::BAD_ARGS, name);

    std::vector<Token> tokens;
    // Вычисление значения, если это встроенная фукнция
    if (isBuiltInFunc(name)) {
        double value = (args.empty()) ? 0.0 : std::stod(args[0].value);
        tokens.emplace_back(std::to_string(funcCalc(toLower(name), value)), Token::DOUBLE);
        return tokens;
    }
    // Проверка на наличие фукнции в таблице проверяется в getArgsCount
    tokens = functions.at(name).second;

    // Замена аргументов в массиве токенов на соответствующие значения
    for (int i = 0; i < argsSize; ++i) {
        for (auto &token: tokens) {
            if (token.tokenType == Token::ARG && token.operatorPriority == i)
                token = args[i];
        }
    }
    return tokens;
}

void FuncHandler::printUndeclaredFunc() const {
    std::cerr << "Вы не объявили функции/переменные:\n";
    bool first = true;
    for (const auto &el: functions) {
        if (!el.second.second.empty()) continue;
        if (first) {
            std::cerr << el.first;
            first = false;
        }
        else std::cerr << ", " << el.first;
    }
    std::cerr << std::endl;
}

double FuncHandler::funcCalc(const std::string &name, const double x) const {
    double res = NAN;
    const double accuracy = 1e-5;
    if (name == "pi") {
        res = M_PI;
    }
    else if (name == "e") {
        res = std::exp(1.0);
    }
    else if (name == "abs") {
        res = std::abs(x);
    }
    else if (name == "sqrt") {
        res = std::sqrt(x);
    }
    else if (name == "ln") {
        res = std::log(x);
    }
    else if (name == "lg") {
        res = std::log10(x);
    }
    else if (name == "log2") {
        res = std::log2(x);
    }
    else if (name == "sign") {
        if (x > 0) res = 1;
        else if (x < 0) res = -1;
        else res = 0;
    }
    else if (name == "exp") {
        res = std::exp(x);
    }
    else if (name == "sin") {
        res = std::sin(x);
    }
    else if (name == "cos") {
        res = std::cos(x);
    }
    else if (name == "tg") {
        if (std::abs(std::cos(x)) >= accuracy)
            res = std::tan(x);
    }
    else if (name == "ctg") {
        if (std::abs(std::sin(x)) >= accuracy)
            res = 1 / std::tan(x);
    }
    else if (name == "arcsin") {
        res = std::asin(x);
    }
    else if (name == "arccos") {
        res = std::acos(x);
    }
    else if (name == "arctg") {
        res = std::atan(x);
    }
    else if (name == "arcctg") {
        res = M_PI / 2 - std::atan(x);
    }
    else throw CalcException(CalcException::UNDECLARED_FUNC, name);
    if (std::isnan(res))
        throw CalcException(CalcException::BAD_ARGS, name);
    return res;
}

std::string FuncHandler::toLower(const std::string &str) {
    std::string res;
    for (const char &c: str)
        res += std::tolower(c);
    return res;
}

bool FuncHandler::isBuiltInFunc(const std::string &name) const {
    return std::find(builtInFunctions.begin(), builtInFunctions.end(), toLower(name)) != builtInFunctions.end();
}

void FuncHandler::clear() {
    functions.clear();
}
