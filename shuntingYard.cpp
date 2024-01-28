#include <string>
#include <vector>
#include <cmath>
#include "token.h"
#include "shuntingYard.h"
using namespace std;

double ShuntingYard::sumUp(const vector<Token> &input) {
    vector<Token> operatorStack;
    vector<double> outputStack;
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
        compute(input, operatorStack, outputStack, token, isSecondLoop);
        i++;
    }
    return outputStack.back();
}

void ShuntingYard::computeForParantheses(vector<double> &outputStack, const Token &token) {
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

void ShuntingYard::compute(const vector<Token> &input, vector<Token> &operatorStack, vector<double> &outputStack, const Token &token,
                           const bool &isSecondLoop) {
    double* buffer = new double[100];
    int bufferTop = 0;
    switch (token.tokenType) {
        case Token::INT:
        case Token::DOUBLE:
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
                            double a = outputStack.back();
                            outputStack.pop_back();
                            double b = outputStack.back();
                            outputStack.pop_back();
                            switch (operatorStack.back().value[0]) {
                                case '+':
                                    outputStack.push_back(a + b);
                                    break;
                                case '-':
                                    if (operatorStack.back().operatorType == Token::BINARY)
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
                            operatorStack.pop_back();
                            if (operatorStack.back().value == "(") break;
                        }
                    }
                    if (operatorStack.back().value == "("){
                        operatorStack.pop_back();
                        if (operatorStack.back().tokenType == Token::FUNC){

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
                computeForParantheses(outputStack, last);
            }
            operatorStack.pop_back();
            break;
        case Token::VAR:
            //sumUp for var
        case Token::FUNC:
            bufferTop = 0;
            operatorStack.push_back(token);
            //представить функцию токенами
            //sumUp
        case Token::SEPARATOR:
            while (operatorStack.back().value != "(") {
                Token last = operatorStack.back();
                operatorStack.pop_back();
                computeForParantheses(outputStack, last);
            }
            buffer[bufferTop] = outputStack.back();
            outputStack.pop_back();
            break;
    }
}
