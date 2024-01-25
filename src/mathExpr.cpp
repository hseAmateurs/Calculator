//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <stdexcept>

#include "MathExpr.h"

std::vector<Token> MathExpr::tokenize(MathExpr::Type type) {
    // Очищаем предыдущие токены
    std::cout << *this << "\n";
    if (type == MAIN) return handleDefinition(*this);
    else return handleDeclaration(*this);
}

std::vector<Token> MathExpr::handleDefinition(const std::string &expr) {
    std::vector<Token> tokens;
    char nextChar = expr[0];
    Token::TokenType tokenType = getCharType(nextChar);
    std::string buffer{nextChar};

    Token::TokenType nextToken;
    size_t exprSize = expr.size();
    // допустимые символы: цифры, буквы, операторы, скобки и запятая
    for (int i = 1; i < exprSize; ++i) {
        nextChar = expr[i];
        if (isspace(nextChar)) continue;
        nextToken = getCharType(nextChar);
        switch (tokenType) {
            /* Токены типа DOUBLE и INT обрабатываются одинаково, за исключением, что
             * если токен уже стал DOUBLE, в нем не может появится ещё одна точка */
            case Token::DOUBLE:
                if (nextToken == Token::DOUBLE) error("Некорректный тип числа");
            case Token::INT:
                switch (nextToken) {
                    case Token::INT:
                    case Token::DOUBLE:
                        buffer += nextChar;
                        break;
                    case Token::OPERATOR:
                    case Token::SEPARATOR:
                    case Token::R_PARANTHESIS:
                        tokens.emplace_back(buffer, tokenType);
                        buffer = nextChar;
                        break;
                    case Token::L_PARANTHESIS:
                        error("Ожидался оператор");
                        break;
                    case Token::VAR:
                        error("Переменные/функции не могут начинаться с цифры");
                        break;
                    default:
                        error("Некорректный символ");
                }
                break;
            case Token::OPERATOR:
                switch (nextToken) {
                    case Token::INT:
                    case Token::DOUBLE:
                    case Token::VAR:
                    case Token::L_PARANTHESIS:
                        if (tokens.empty() || tokens.back().tokenType == Token::SEPARATOR ||
                            tokens.back().tokenType == Token::L_PARANTHESIS)
                            tokens.emplace_back(buffer, tokenType, Token::UNARY);
                        else tokens.emplace_back(buffer, tokenType, Token::BINARY, getOperatorPriority(buffer[0]));
                        buffer = nextChar;
                        break;
                    case Token::R_PARANTHESIS:
                        error("Ожидалась открывающая скобка");
                        break;
                    default:
                        error("Синтаксическая ошибка");
                }
                break;
            case Token::VAR:
                switch (nextToken) {
                    case Token::VAR:
                    case Token::INT:
                        buffer += nextChar;
                        break;
                    case Token::OPERATOR:
                    case Token::SEPARATOR:
                    case Token::R_PARANTHESIS:
                        tokens.emplace_back(buffer, tokenType);
                        buffer = nextChar;
                        break;
                    case Token::L_PARANTHESIS:
                        tokenType = Token::FUNC;
                        tokens.emplace_back(buffer, tokenType);
                        buffer = nextChar;
                        break;
                    default:
                        error("Синтаксическая ошибка");
                }
                break;
            case Token::SEPARATOR:
            case Token::L_PARANTHESIS:
                switch (nextToken) {
                    case Token::INT:
                    case Token::VAR:
                    case Token::L_PARANTHESIS:
                        tokens.emplace_back(buffer, tokenType);
                        buffer = nextChar;
                        break;
                    case Token::OPERATOR:
                        if (nextChar != '-') error("Синтаксическая ошибка");
                        tokens.emplace_back(buffer, tokenType);
                        buffer = nextChar;
                        break;
                    case Token::R_PARANTHESIS:
                        if (tokenType == Token::SEPARATOR)
                            error("Ожидалась открывающая скобка");
                        else error("Выражение в скобках пустое");
                        break;
                    default:
                        error("Синтаксическая ошибка");
                }
                break;
            case Token::R_PARANTHESIS:
                switch (nextToken) {
                    case Token::OPERATOR:
                    case Token::SEPARATOR:
                    case Token::R_PARANTHESIS:
                        tokens.emplace_back(buffer, tokenType);
                        buffer = nextChar;
                        break;
                    case Token::INT:
                    case Token::VAR:
                    case Token::L_PARANTHESIS:
                        error("Ожидался оператор");
                        break;
                    default:
                        error("Синтаксическая ошибка");
                }
                break;
            default:
                std::cerr << "Unexpected way in tokenization";
        }
        if(tokenType != Token::DOUBLE)
            tokenType = nextToken;
    }
    switch (tokenType) {
        case Token::INT:
        case Token::DOUBLE:
        case Token::VAR:
        case Token::R_PARANTHESIS:
            tokens.emplace_back(buffer, tokenType);
            break;
        default:
            error("Синтаксическая ошибка");
    }

    return tokens;
}

std::vector<Token> MathExpr::handleDeclaration(const std::string &expr) {
    // Получаем всё до знака =
    std::string declaration = expr.substr(0, expr.find('='));
    if(declaration.empty()) error("Не объявлена функция");
    // Получаем всё после знака =
    std::string definition = expr.substr(expr.find('=') + 1);

    std::vector<Token> declr_tokens = handleDefinition(declaration);
    Token::TokenType token = declr_tokens[0].tokenType;

    size_t size = declr_tokens.size();
    if(size <= 3) error("Ошибка в объявлени функции");

    Token::TokenType nextToken;
    for (int i = 1; i < size; ++i) {
        nextToken = declr_tokens[i].tokenType;

    }
}

void MathExpr::error(const std::string &msg) const {
    std::cerr << msg << std::endl;
    exit(-1);
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

Token::TokenType MathExpr::getCharType(const char &ch) const {
    if (isdigit(ch)) return Token::INT;
    else if (ch == '.') return Token::DOUBLE;
    else if (isalpha(ch)) return Token::VAR;
    else if (ch == '(') return Token::L_PARANTHESIS;
    else if (ch == ')') return Token::R_PARANTHESIS;
    else if (ch == ',') return Token::SEPARATOR;
    else if (isOperator(ch)) return Token::OPERATOR;
    else return Token::UNDEFINED;
}

bool MathExpr::isOperator(const char &target) const {
    for (const auto &it: operatorPriority)
        if (target == it.first) return true;
    return false;
}
