#include <vector>
#include "token.h"
#include "funcHandler.h"

using namespace std;

class ShuntingYard {
public:

    ShuntingYard(FuncHandler &funcHandler) : funcHandler(funcHandler) { };

    double sumUp(const vector<Token> &input);

private:
    void computeOnce(vector<double> &outputStack, const Token &token);

    void
    compute(vector<vector<double>> &buffer, vector<Token> &operatorStack, vector<double> &outputStack, const Token &token,
            const bool &isSecondLoop, bool &isFunction, int &funcCount);

    FuncHandler funcHandler;
};
