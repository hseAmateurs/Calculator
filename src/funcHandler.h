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

    bool isFuncValid(std::string &name, int argsCount) const;

    std::vector<Token> getFunc(std::string &name, std::vector<Token>& args);

    void printUndeclaredFunc() const;

private:
    bool error(const std::string &msg) const;

    std::map<std::string, std::pair<int, std::vector<Token>>> functions;

    int emptyRows = 0;
};


#endif //CALCULATOR_FUNCHANDLER_H
