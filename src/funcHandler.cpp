//
// Created by Shon on 26.01.2024.
//

#include "funcHandler.h"

#include <iostream>

bool FuncHandler::addFunc(std::string &name, const int argsCount, const std::vector<Token> tokens) {
    // Если функция уже занесена в список
    std::cout << "addFunc: " << name << " " << argsCount << " " << tokens.size() << "\n";
    if (functions.find(name) != functions.end()) {
        emptyRows--;
        // Скипаем т.к. пустая фукнция уже есть в словаре
        if(tokens.empty()) return true;
        // И она объявлена
        if (!functions.at(name).second.empty()) {
            if (tokens.empty()) return true;
            else return error("Функция/переменная уже объявлена");
        }
    }
    functions[std::move(name)] = std::make_pair(argsCount, tokens);
    std::cout << "Current map: " << emptyRows << "\n";
    for (const auto& el : functions) {
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
