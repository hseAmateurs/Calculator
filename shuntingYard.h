#include <vector>
#include "token.h"
using namespace std;

class ShuntingYard
{
public:
    double sumUp(const vector<Token> &input);

private:
    void computeForParantheses(vector<double> &outputStack, const Token &token);
    void compute(const vector<Token> &input, vector<Token> &operatorStack, vector<double> &outputStack, const Token &token,
                 const bool &isSecondLoop);
};