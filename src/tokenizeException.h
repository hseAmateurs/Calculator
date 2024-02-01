//
// Created by Shon on 29.01.2024.
//

#ifndef CALCULATOR_TOKENIZEEXCEPTION_H
#define CALCULATOR_TOKENIZEEXCEPTION_H

#include <utility>

#include "calcException.h"

class TokenizeException : public CalcException {
public:
    explicit TokenizeException (Type type, std::string message = "") : CalcException(type, std::move(message)) { };
};

#endif //CALCULATOR_TOKENIZEEXCEPTION_H
