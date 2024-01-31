#include <iostream>
#include <string>
#include <cmath>
#include "token.h"
#include "shuntingYard.h"
#include "calcException.h"

using namespace std;

bool ShuntingYard::areParentheses(const vector<Token> &input) {
    vector<Token> parenthesesStack;
    for (const Token& token: input){
        if (token.tokenType == Token::L_PARANTHESIS)
            parenthesesStack.push_back(token);
        else if (token.tokenType == Token::R_PARANTHESIS){
            if (parenthesesStack.back().tokenType == Token::L_PARANTHESIS)
                parenthesesStack.pop_back();
            else
                return false;
        }
    }
    if (parenthesesStack.empty()) return true;
    return false;
}

double ShuntingYard::sumUp(const vector<Token> &input) {
    if (!areParentheses(input)){
        throw CalcException(CalcException::SYNTAX_ERROR, "Неправильное расположение скобок");
    }
    vector<Token> operatorStack;
    vector<double> outputStack;
    vector<vector<double>> buffer;
    Token token;
    int i = 0;
    bool isSecondLoop = false;
    for (;;) {
        if (i < input.size())
            token = input[i];
        else {
            isSecondLoop = true;
            if (operatorStack.empty()) break;
            token = operatorStack.back();
        }
        compute(buffer, operatorStack, outputStack, token, isSecondLoop);
        i++;
    }
    return outputStack.back();
}

void ShuntingYard::computeOnce(vector<double> &outputStack, const Token &token) {
    double a = outputStack.back();
    outputStack.pop_back();
    double b = outputStack.back();
    outputStack.pop_back();
    switch (token.operatorType) {
        case Token::UNARY:
            outputStack.push_back(b);
            outputStack.push_back(-a);
            break;
        case Token::BINARY:
            switch (token.value[0]) {
                case '+':
                    outputStack.push_back(a + b);
                    break;
                case '-':
                    outputStack.push_back(b - a);
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
                default:
                    throw CalcException(CalcException::UNEXPECTED_ERROR, token.value);
            }
            break;
        default:
            throw CalcException(CalcException::UNEXPECTED_ERROR, token.value);
    }
}

void ShuntingYard::compute(vector<vector<double>> &buffer, vector<Token> &operatorStack, vector<double> &outputStack,
                           const Token &token,
                           const bool &isSecondLoop) {
    switch (token.tokenType) {
        case Token::INT:
        case Token::DOUBLE:
            if (!buffer.empty())
                buffer.back().push_back(stod(token.value));
            else
                outputStack.push_back(stod(token.value));
            break;
        case Token::OPERATOR:
            switch (token.operatorType) {
                case Token::UNARY:
                    if (!isSecondLoop)
                        operatorStack.push_back(token);
                    else{
                        outputStack.back() = -1 * outputStack.back();
                        operatorStack.pop_back();
                    }
                    break;
                case Token::BINARY:
                    if (!operatorStack.empty() && operatorStack.back().operatorPriority >= token.operatorPriority &&
                        operatorStack.back().value != "(") {
                        while (!operatorStack.empty() &&
                               operatorStack.back().operatorPriority >= token.operatorPriority) {
                            computeOnce(!buffer.empty() ? buffer.back() : outputStack, operatorStack.back());
                            operatorStack.pop_back();
                        }
                    }
                    if (!isSecondLoop)
                        operatorStack.push_back(token);
                    break;
                default:
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
                computeOnce(!buffer.empty() ? buffer.back() : outputStack, last);
            }
            operatorStack.pop_back();
            if (operatorStack.back().tokenType == Token::FUNC) {
                vector<Token> args;
                for (const auto &val: buffer.back())
                    args.emplace_back(to_string(val), Token::DOUBLE);
                args = funcHandler.getFunc(operatorStack.back().value, args);
                for (const Token& func: isFunction){
                    for (const Token& arg: args) {
                        if (arg.tokenType == Token::FUNC && func.value == arg.value) {
                            throw CalcException(CalcException::CYCLIC_FUNC, arg.value);
                        }
                    }
                }
                buffer.pop_back();
                if (!buffer.empty()) {
                    buffer.back().push_back(sumUp(args));
                }
                else
                    outputStack.push_back(sumUp(args));
                isFunction.pop_back();
                operatorStack.pop_back();
            }
            break;
        case Token::VAR:
            !buffer.empty() ?
                buffer.back().push_back(sumUp(funcHandler.getFunc(token.value, {}))) :
                outputStack.push_back(sumUp(funcHandler.getFunc(token.value, {})));
            break;
        case Token::FUNC:
            isFunction.push_back(token);
            buffer.emplace_back();
            operatorStack.push_back(token);
            break;
        case Token::SEPARATOR:
            while (operatorStack.back().tokenType != Token::L_PARANTHESIS &&
                   operatorStack.back().tokenType != Token::SEPARATOR) {
                Token last = operatorStack.back();
                operatorStack.pop_back();
                computeOnce(!buffer.empty() ? buffer.back() : outputStack, last);
            }
            operatorStack.push_back(token);
            break;
        default:
            break;
    }
}
