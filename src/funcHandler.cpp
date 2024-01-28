//
// Created by Shon on 26.01.2024.
//

#include "funcHandler.h"

#include <iostream>
#include <valarray>
#include <math.h>

bool FuncHandler::addFunc(std::string &name, const int argsCount, const std::vector<Token> tokens) {
    // Дебаг информация
    std::cout << "addFunc: " << name << " " << argsCount << " " << tokens.size() << "\n";
    if (isBuiltInFunc(name)) {
        if (tokens.empty()) {
            emptyRows--;
            return true;
        }
        else return error("Вы не можете переобъявить встроенные функции/константы");
    }
    // Если функция уже занесена в список
    if (functions.find(name) != functions.end()) {
        emptyRows--;
        // Скипаем т.к. пустая фукнция уже есть в таблицеы
        if (tokens.empty()) return true;
        // И она объявлена
        if (!functions.at(name).second.empty()) {
            if (tokens.empty()) return true;
            else return error("Функция/переменная уже объявлена");
        }
    }
    functions[std::move(name)] = std::make_pair(argsCount, tokens);
    // Дебаг информация
    std::cout << "Current map: " << emptyRows << "\n";
    for (const auto &el: functions) {
        std::cout << el.first << " " << el.second.first << " " << el.second.second.size() << "\n";
    }
    std::cout << std::endl;
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
    if (functions.find(name) == functions.end()) {
        error("Функция не объявлена");
        return -1;
    }
    return functions.at(name).first;
}

std::vector<Token> FuncHandler::getFunc(const std::string &name, const std::vector<Token> &args) {
    size_t argsSize = args.size();

    // Проверка количества подаваемых аргументов с необходимым
    if(argsSize != getArgsCount(name))
        error("Некорректное количество аргументов функции");

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

double FuncHandler::funcCalc(const std::string &name, const double x) const {
    double res = 0;
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
        res = std::tan(x);
    }
    else if (name == "ctg") {
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
    else error("Неизвестная функция");
    if (std::isnan(res))
        error("Вы не учли область определения функции");
    return res;
}

std::string FuncHandler::toLower(const std::string &str) const {
    std::string res;
    for (const char &c: str)
        res += std::tolower(c);
    return res;
}

bool FuncHandler::isBuiltInFunc(const std::string &name) const {
    return std::find(builtInFunctions.begin(), builtInFunctions.end(), toLower(name)) != builtInFunctions.end();
}
