#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "token.h"
using namespace std;



double sumUp(const vector<Token>& input){
    vector<Token> operatorStack;
    vector<double> outputStack;
    Token token = Token("0", Token::INT);
    int i = 0;
    for (;;){
        if (i < input.size())
            token = input[i];
        else
            token = operatorStack[i % input.size()];
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
                        if (!operatorStack.empty() && operatorStack.back().operatorPriority >= token.operatorPriority && operatorStack.back().value != "(") {
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
                                        outputStack.push_back(a - b);
                                        break;
                                    case '*':
                                        outputStack.push_back(a * b);
                                        break;
                                    case '/':
                                        outputStack.push_back(a / b);
                                        break;
                                    case '^':
                                        outputStack.push_back(a + b);
                                        break;
                                }
                                if (operatorStack.back().value == "(") break;
                            }
                        }
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
                break;
            case Token::VAR:
                break;
            case Token::FUNC:
                break;
            case Token::SEPARATOR:
                break;
        }
        i++;
    }

    return outputStack.back();
}

int main(){
    vector<int> outputStack;
    vector<Token> operatorStack;
    vector<Token> tokens;
    tokens.push_back(Token("12", Token::INT));
    tokens.push_back(Token("+", Token::OPERATOR, Token::BINARY, 1));
    tokens.push_back(Token("12", Token::INT));
    tokens.push_back(Token("*", Token::OPERATOR, Token::BINARY, 2));
    //tokens.push_back(Token("(", Token::L_PARANTHESIS, Token::UNARY, 4));
    //tokens.push_back(Token("-", Token::OPERATOR, Token::UNARY, 3));
    tokens.push_back(Token("3", Token::INT));
    //tokens.push_back(Token(")", Token::R_PARANTHESIS, Token::UNARY, 4));
    cout << sumUp(tokens);
}
