//
// Created by Shon on 17.01.2024.
//

#include <iostream>
#include <stdexcept>

#include "mathExpr.h"
#include "tokenizeException.h"

std::tuple<std::vector<Token>, std::string, int> MathExpr::tokenize(MathExpr::Type type) {
    if (type == MAIN)
        return std::make_tuple(handleDefinition(*this), "", 0);
    else
        return handleDeclaration(*this);
}

std::vector<Token> MathExpr::handleDefinition(const std::string &expr) {
    std::vector<Token> tokens;
    int i = 0;
    //Пропуск пробелов в начале строки
    while (isspace(expr[i++]));
    char nextChar = expr[i - 1];
    Token::TokenType tokenType = getCharType(nextChar);
    std::string buffer{nextChar};

    Token::TokenType nextToken;
    size_t exprSize = expr.size();
    // допустимые символы: цифры, буквы, операторы, скобки и запятая
    for (; i < exprSize; ++i) {
        nextChar = expr[i];
        if (isspace(nextChar)) continue;
        nextToken = getCharType(nextChar);
        switch (tokenType) {
            /* Токены типа DOUBLE и INT обрабатываются одинаково, за исключением, что
             * если токен уже стал DOUBLE, в нем не может появится ещё одна точка */
            case Token::DOUBLE:
                if (nextToken == Token::DOUBLE)
                    throw TokenizeException(TokenizeException::BAD_NUM, buffer+nextChar);
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
                        tokenType = nextToken;
                        break;
                    case Token::L_PARANTHESIS:
                        throw TokenizeException(TokenizeException::EXPECTED_OPERATOR, buffer+nextChar);
                    case Token::VAR:
                        throw TokenizeException(TokenizeException::BAD_FUNC_NAME, buffer+nextChar);
                    default:
                        throw TokenizeException(TokenizeException::BAD_CHAR, buffer+nextChar);
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
                            tokens.emplace_back(buffer, tokenType, 0, Token::UNARY);
                        else tokens.emplace_back(buffer, tokenType, getOperatorPriority(buffer[0]), Token::BINARY);
                        buffer = nextChar;
                        break;
                    case Token::R_PARANTHESIS:
                        throw TokenizeException(TokenizeException::EXPECTED_L_PARANTHESIS, buffer+nextChar);
                    default:
                        throw TokenizeException(TokenizeException::SYNTAX_ERROR, buffer+nextChar);
                }
                tokenType = nextToken;
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
                        throw TokenizeException(TokenizeException::SYNTAX_ERROR, buffer+nextChar);
                }
                if(nextToken != Token::INT)
                    tokenType = nextToken;
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
                        if (nextChar != '-')
                            throw TokenizeException(TokenizeException::SYNTAX_ERROR, buffer+nextChar);
                        tokens.emplace_back(buffer, tokenType);
                        buffer = nextChar;
                        break;
                    case Token::R_PARANTHESIS:
                        if (tokenType == Token::SEPARATOR)
                            throw TokenizeException(TokenizeException::EXPECTED_L_PARANTHESIS, buffer+nextChar);
                        else
                            throw TokenizeException(TokenizeException::EMPTY_PARANTHESIS, buffer+nextChar);
                    default:
                        throw TokenizeException(TokenizeException::SYNTAX_ERROR, buffer+nextChar);
                }
                tokenType = nextToken;
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
                        throw TokenizeException(TokenizeException::EXPECTED_OPERATOR, buffer+nextChar);
                        break;
                    default:
                        throw TokenizeException(TokenizeException::SYNTAX_ERROR, buffer+nextChar);
                }
                tokenType = nextToken;
                break;
            default:
                throw TokenizeException(TokenizeException::UNEXPECTED_ERROR, buffer+nextChar);
        }
    }
    // Обработка последнего токена
    switch (tokenType) {
        case Token::INT:
        case Token::DOUBLE:
        case Token::VAR:
        case Token::R_PARANTHESIS:
            tokens.emplace_back(buffer, tokenType);
            break;
        default:
            throw TokenizeException(TokenizeException::SYNTAX_ERROR, buffer);
    }

    return tokens;
}

std::tuple<std::vector<Token>, std::string, int> MathExpr::handleDeclaration(const std::string &expr) {
    // Получаем всё до знака =
    std::string declaration = expr.substr(0, expr.find('='));
    if (declaration.empty())
        throw TokenizeException(TokenizeException::UNDECLARED_FUNC, declaration);
    // Получаем всё после знака =
    std::string definition = expr.substr(expr.find('=') + 1);

    std::vector<Token> declrTokens = handleDefinition(declaration);
    if (!isDeclarationValid(declrTokens))
        throw TokenizeException(TokenizeException::BAD_FUNC_DECLARATION, declaration);

    std::vector<Token> defTokens = handleDefinition(definition);
    // Выражение — переменная
    if (declrTokens.size() == 1)
        return std::make_tuple(defTokens, declrTokens[0].value, 0);

    // Замена аргументов в правой части на соотвествующие токены
    char argPriority = 0;
    for (const auto &arg: declrTokens) {
        if (arg.tokenType != Token::VAR) continue;
        for (auto &token: defTokens) {
            if (token.value != arg.value) continue;
            token.tokenType = Token::ARG;
            token.operatorPriority = argPriority;
        }
        argPriority++;
    }
    return std::make_tuple(defTokens, declrTokens[0].value, argPriority);
}

bool MathExpr::isDeclarationValid(const std::vector<Token> &tokens) const {
    Token::TokenType tokenType = tokens[0].tokenType;
    size_t size = tokens.size();

    bool isFunc = (size >= 3 && tokenType == Token::FUNC);
    bool isVar = (size == 1 && tokenType == Token::VAR);
    if (isVar)
        return true;
    else if (!isFunc)
        return false;

    Token::TokenType nextToken;
    for (int i = 1; i < size; ++i) {
        nextToken = tokens[i].tokenType;
        switch (tokenType) {
            case Token::FUNC:
                if (nextToken != Token::L_PARANTHESIS)
                    return false;
                break;
            case Token::L_PARANTHESIS:
            case Token::SEPARATOR:
                if (nextToken != Token::VAR)
                    return false;
                break;
            case Token::VAR:
                if (nextToken != Token::SEPARATOR && nextToken != Token::R_PARANTHESIS)
                    return false;
                break;
            default:
                return false;
        }
        tokenType = nextToken;
    }
    if (tokenType != Token::R_PARANTHESIS)
        return false;
    return true;
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
