//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "MathExpr.h"

MathExpr::MathExpr() : std::string() {
    // Переносим ключи из operationPriority в отдельный массив
    for (const auto &it: operatorPriority)
        operators.push_back(it.first);
}

std::vector<Token> MathExpr::tokenize(MathExpr::Type type) {
    // Очищаем предыдущие токены
    tokens.clear();
    std::cout << *this << "\n";
    if (type == MAIN) handleMainExpr();
    return tokens;
}

char MathExpr::getOperatorPriority(const char &operatorName) const {
    char priority;
    try {
        priority = operatorPriority.at(operatorName);
    } catch (const std::out_of_range &ex) {
        std::cerr << "There's no operator: " << operatorName << "\n";
        exit(-1);
    }
    return priority;
}

void MathExpr::handleMainExpr() {
    Token::TokenType tokenType;
    std::string buffer;
    for (const auto &ch: *this) {
        if (isspace(ch)) continue;
        // Начинаем считывать новый токен
        if (buffer.empty()) {
            if (isdigit(ch)) {
                tokenType = Token::INT;
                buffer += ch;
            }
            else if (isOperator(ch)) {
                if (ch == '-') {
                    if (tokens.empty() || tokens.back().tokenType == Token::L_PARANTHESIS ||
                            tokens.back().tokenType == Token::SEPARATOR)
                        tokens.emplace_back("-", Token::OPERATOR, Token::UNARY);
                    else
                        error("Вы не поставили скобки перед унарным минусом");
                }
                else {
                    if (tokens.empty()) error("Выражение не может начинаться с оператора");
                    else error("Не может идти два оператора подряд");
                }
            }
            else if (ch == '(') {
                if (tokens.empty() || tokens.back().tokenType == Token::OPERATOR ||
                    tokens.back().tokenType == Token::L_PARANTHESIS ||
                    tokens.back().tokenType == Token::SEPARATOR)
                    tokens.emplace_back("(", Token::L_PARANTHESIS);
                else error("Отсутсвует оператор между выражениями");
            }
            else if (isalpha(ch)) {
                tokenType = Token::VAR;
                buffer += ch;
            }
            else if (ch == ')') {
                if (tokens.empty())
                    error("Выражение не может начинаться с закрывающей скобки");
                else if (tokens.back().tokenType == Token::L_PARANTHESIS)
                    error("Пустое выражение в скобках");
            }
            else error("Неизвестный символ");
            continue;
        }
        // Продолжаем считывать токен
        switch (tokenType) {
            /* Токены типа DOUBLE и INT обрабатываются одинаково, за исключением, что
             * если токен уже стал DOUBLE, в нем не может появится ещё одна точка */
            case Token::DOUBLE:
                if (ch == '.') error("Мне неизвестен такой тип чисел");
            case Token::INT:
                if (isdigit(ch)) buffer += ch;
                else if (ch == '.') {
                    tokenType = Token::DOUBLE;
                    buffer += ch;
                }
                else if (isOperator(ch)) {
                    tokens.emplace_back(buffer, tokenType);
                    tokenType = Token::OPERATOR;
                    buffer = ch;
                    tokens.emplace_back(buffer, tokenType, Token::BINARY, getOperatorPriority(ch));
                    buffer.clear();
                }
                else if (ch == ',') {
                    tokens.emplace_back(buffer, tokenType);
                    tokenType = Token::SEPARATOR;
                    buffer = ch;
                    tokens.emplace_back(buffer, tokenType);
                    buffer.clear();
                }
                else if (ch == '(') error("Ожидался оператор");
                else if (ch == ')') {
                    tokens.emplace_back(buffer, tokenType);
                    tokenType = Token::R_PARANTHESIS;
                    buffer = ch;
                    tokens.emplace_back(buffer, tokenType);
                    buffer.clear();
                }
                else if (isalpha(ch)) error("Переменные/функции не могут начинаться с цифры");
                else error("Неизвестный символ");
                break;
            case Token::VAR:
                if (isdigit(ch) || isalpha(ch)) buffer += ch;
                else if (isOperator(ch)) {
                    tokens.emplace_back(buffer, tokenType);
                    tokenType = Token::OPERATOR;
                    buffer = ch;
                    tokens.emplace_back(buffer, tokenType, Token::BINARY, getOperatorPriority(ch));
                    buffer.clear();
                }
                else if (ch == ',') {
                    tokens.emplace_back(buffer, tokenType);
                    tokenType = Token::SEPARATOR;
                    buffer = ch;
                    tokens.emplace_back(buffer, tokenType);
                    buffer.clear();
                }
                else if (ch == '(') {
                    tokenType = Token::FUNC;
                    tokens.emplace_back(buffer, tokenType);
                    tokenType = Token::L_PARANTHESIS;
                    buffer = ch;
                    tokens.emplace_back(buffer, tokenType);
                    buffer.clear();
                }
                else if (ch == ')') error("Ожидался оператор/открывающая скобка");
                else error("Недопустимые символы для именования перменной/функции");
                break;
            default:
                std::cerr << "Unexpected way in tokenization";
        }
    }
}

void MathExpr::handleSecondaryExpr(std::vector<Token> &tokens) {

}

bool MathExpr::isOperator(const char &target) const {
    return std::find(operators.begin(), operators.end(), target) != operators.end();
}

void MathExpr::error(const std::string &msg) const {
    std::cerr << msg << std::endl;
    exit(-1);
}
