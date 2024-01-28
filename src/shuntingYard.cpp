#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "token.h"
#include "shuntingYard.h"
#include "funcHandler.h"
using namespace std;

double ShuntingYard::sumUp(const vector<Token> &input) {
    vector<Token> operatorStack;
    vector<double> outputStack;
    vector<vector<double>> buffer;
    bool isFunction = false;
    int funcCount = 0;
    Token token = Token("0", Token::INT);
    int i = 0;
    bool isSecondLoop = false;
    for (;;) {
        if (i < input.size())
            token = input[i];
        else {
            isSecondLoop = true;
            if (!operatorStack.empty())
                token = operatorStack.back();
            else
                break;
        }
        compute(buffer, operatorStack, outputStack, token, isSecondLoop, isFunction, funcCount);
        i++;
    }
    return outputStack.back();
}

void ShuntingYard::computeOnce(vector<double> &outputStack, const Token &token) {
    switch (token.operatorType) {
        case Token::UNARY:
        case Token::BINARY:
            double a = outputStack.back();
            outputStack.pop_back();
            double b = outputStack.back();
            outputStack.pop_back();
            switch (token.value[0]) {
                case '+':
                    outputStack.push_back(a + b);
                    break;
                case '-':
                    if (token.operatorType == Token::BINARY)
                        outputStack.push_back(b - a);
                    else {
                        outputStack.push_back(b);
                        outputStack.push_back(-a);
                    }
                    break;
                case '*':
                    outputStack.push_back(a * b);
                    break;
                case '/':
                    outputStack.push_back(b / a);
                    break;
                case '^':
                    outputStack.push_back(pow(b, a));
                    break;
            }
    }
}

void ShuntingYard::compute(vector<vector<double>> &buffer, vector<Token> &operatorStack, vector<double> &outputStack, const Token &token,
                           const bool &isSecondLoop, bool &isFunction, int &funcCount) {
    switch (token.tokenType) {
        case Token::INT:
        case Token::DOUBLE:
            isFunction ?
            buffer.back().push_back(stod(token.value)) :
            outputStack.push_back(stod(token.value));
            break;
        case Token::OPERATOR:
            switch (token.operatorType) {
                case Token::UNARY:
                    operatorStack.push_back(token);
                    break;
                case Token::BINARY:
                    if (!operatorStack.empty() && operatorStack.back().operatorPriority >= token.operatorPriority &&
                        operatorStack.back().value != "(") {
                        while (operatorStack.back().operatorPriority >= token.operatorPriority) {
                            computeOnce(isFunction ? buffer.back() : outputStack, token);
                            operatorStack.pop_back();
                        }
                    }
                    if (!isSecondLoop)
                        operatorStack.push_back(token);
                    break;
                case Token::NONE:
                    break;
            }
            break;
        case Token::L_PARANTHESIS:
            operatorStack.push_back(token);
            break;
        case Token::R_PARANTHESIS:
            while (operatorStack.back().value != "(") {
                Token last = operatorStack.back();
                operatorStack.pop_back();
                if (last.tokenType == Token::SEPARATOR) continue;
                computeOnce(isFunction ? buffer.back() : outputStack, token);
            }
            operatorStack.pop_back();
            if (operatorStack.back().tokenType == Token::FUNC) {
                if (funcCount == 0) isFunction = false;
                else funcCount--;
                int argsCount = funcHandler.getArgsCount(operatorStack.back().value);
                if (argsCount != buffer.back().size()) {
                    std::cerr << "Wrong arg count";
                    exit(-1);
                }
                vector<Token> args;
                for (const auto &val: buffer.back())
                    args.emplace_back(to_string(val), Token::DOUBLE);
                args = funcHandler.getFunc(operatorStack.back().value, args);
                if (isFunction){
                    buffer.pop_back();
                    buffer.back().push_back(sumUp(args));
                } else {
                    outputStack.push_back(sumUp(args));
                }
                operatorStack.pop_back();
            }
            break;
        case Token::VAR:
            outputStack.push_back(sumUp(funcHandler.getFunc(token.value, {})));
            break;
        case Token::FUNC:
            if (isFunction) funcCount++;
            buffer.emplace_back();
            isFunction = true;
            operatorStack.push_back(token);
            break;
        case Token::SEPARATOR:
            while (operatorStack.back().tokenType != Token::L_PARANTHESIS && operatorStack.back().tokenType != Token::SEPARATOR) {
                Token last = operatorStack.back();
                operatorStack.pop_back();
                computeOnce(isFunction ? buffer.back() : outputStack, last);
            }
            operatorStack.push_back(token);
            break;
        case Token::NONE:
            break;
    }
}