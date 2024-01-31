#include <vector>
#include "token.h"
#include "funcHandler.h"

using namespace std;

class ShuntingYard {
public:

    ShuntingYard(FuncHandler *funcHandler) : funcHandler(funcHandler), isFunction(vector<Token>()) { };

    double sumUp(const vector<Token> &input);

    void clear();

private:
    void computeOnce(vector<double> &outputStack, const Token &token);

    bool areParentheses(const vector<Token> &input);

    void
    compute(vector<vector<double>> &buffer, vector<Token> &operatorStack, vector<double> &outputStack, const Token &token,
            const bool &isSecondLoop);

    FuncHandler *funcHandler;
    vector<Token> isFunction;
};
