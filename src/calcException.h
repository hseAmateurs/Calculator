//
// Created by Shon on 29.01.2024.
//

#ifndef CALCULATOR_CALCEXCEPTION_H
#define CALCULATOR_CALCEXCEPTION_H

#include <string>
#include <vector>
#include <utility>

class CalcException : public std::exception {
public:
    enum Type {
        BAD_NUM,
        EXPECTED_OPERATOR,
        BAD_CHAR,
        BAD_FUNC_NAME,
        EXPECTED_L_PARANTHESIS,
        SYNTAX_ERROR,
        EMPTY_PARANTHESIS,
        UNDECLARED_FUNC,
        BAD_FUNC_DECLARATION,
        UNEXPECTED_ERROR,
        BUILTIN_REDECLARATION,
        ALREADY_DECLARED,
        BAD_ARGS,
        DUBLICATE_MAIN,
        NO_MAIN
    };

    explicit CalcException(Type type, std::string message = "") : type(type), message(std::move(message)) { };

    std::string desc() const { return descriptions.at(type); };

    const Type type;

    const std::string message;

private:
    const std::vector<std::string> descriptions = {
            "Некорректное число",
            "Ожидался оператор",
            "Некорректный символ",
            "Некорректное название фукнции/переменной",
            "Ожидалась открывающая скобка",
            "Синтаксическая ошибка",
            "Пустое выражение в скобках",
            "Функция/переменная не объявлена",
            "Некорректное объявление фукнции",
            "Неожиданная ошибка",
            "Вы не можете переобъявить встроенные функции/константы",
            "Функция/переменная уже объявлена",
            "Некорректные аргументы функции",
            "Вы уже вводили главное выражение",
            "Вы не ввели основное выражение"
    };
};


#endif //CALCULATOR_CALCEXCEPTION_H
