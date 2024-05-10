#include "expression.h"
#include <queue>
#include <string>
#include <stack>
#include "OperationSet.h"

class Token {
    double num;
    char op = '0';
public:
    Token(double num, char op) : num(num), op(op) {};
    bool isOp() const {
        return op != '0';
    }
    char getOp() const {
        return op;
    }
    double getNum() const {
        return num;
    }
    bool isBracket() const {
        return op == '(' || op == ')';
    }
};

enum class TokenType {
    nothing,
    number,
    bracket,
    op
};

std::queue<Token> parseExpression(const char* expression) {
    std::queue<Token> tokenQueue;
    TokenType last = TokenType::nothing;
    TokenType current = TokenType::nothing;
    std::string token("");
    int i = 0;

    while (expression[i] != '\0') {
        if ((expression[i] <= '9' && expression[i] >= '0') || expression[i] == '-' || expression[i] == '.') {
            if (token != "" && !isdigit(token[0]) && token[0] != '-' && token[0] != '.') {
                throw incorrect_expression("There has to be a space!");
            }
            token += expression[i];
            current = TokenType::number;
        }
        else if (expression[i] == ' ') {
            if (current == TokenType::nothing) {
                i++;
                continue;
            }
            if (token == "-") {
                throw incorrect_expression("There can't be a gap after the minus sign!");
            }
            if (last == current && current != TokenType::bracket && last != TokenType::nothing) {
                throw incorrect_expression("Can't have 2 consecutive numbers or operators!");
            }
            last = current;

            if (current == TokenType::number) {
                tokenQueue.push(Token(std::stod(token), '0'));
            }
            else if (current == TokenType::bracket || current == TokenType::op) {
                tokenQueue.push(Token(0, token[0]));
            }
           
            token = "";
            current = TokenType::nothing;
        }
        else if (expression[i] == '(' || expression[i] == ')') {
            current = TokenType::bracket;
            if (token != "") {
                throw incorrect_expression("There has to be a space!");
            }
            token += expression[i];
        }
        else if ((expression[i] >= 'a' && expression[i] <= 'z')) {
            current = TokenType::op;
            token += expression[i];
        }
        else if (expression[i] >= 'A' && expression[i] <= 'Z') {
            current = TokenType::op;
            token += expression[i] + ('a' - 'A');
        }
        

        i++;
    }
    if (last == current && current != TokenType::bracket && last != TokenType::nothing) {
            throw incorrect_expression("Can't have 2 consecutive numbers or operators!");
    }
    
    if (token[0] != ' ' && token != "") {
        if (current == TokenType::number) {
            tokenQueue.push(Token(std::stod(token), '0'));
        }
        else if (current == TokenType::bracket || current == TokenType::op) {
            tokenQueue.push(Token(0, token[0]));
        }
    }
    

    return tokenQueue;
}

double calculate(const char* expression, const OperationSet& operators) {
    std::queue<Token> tokenQueue = parseExpression(expression);
    
    std::stack<char> operatorStack;
    std::stack<double> outputStack;

    if (tokenQueue.empty()) {
        return 0;
    }

    while (!tokenQueue.empty()) {
        Token token = tokenQueue.front();
        tokenQueue.pop();

        if (!token.isOp()) {
            outputStack.push(token.getNum());
        }
        else if (token.isOp() && !token.isBracket()) {
            if (!operators.contains(token.getOp())) {
                throw incorrect_expression("Invalid operator!");
            }

            while (!operatorStack.empty() && operatorStack.top() != '(' && operators.get(token.getOp()) < operators.get(operatorStack.top())) {
                
                char operation = operatorStack.top();
                operatorStack.pop();

                if (outputStack.empty()) {
                    throw incorrect_expression("Incorrect expression!");
                }
                double right = outputStack.top();
                outputStack.pop();

                if (outputStack.empty()) {
                    throw incorrect_expression("Incorrect expression!");
                }
                double left = outputStack.top();
                outputStack.pop();

                outputStack.push(operators.get(operation).calculate(left, right));
            }
            operatorStack.push(token.getOp());

        }
        else if (token.getOp() == '(') {
            if (!tokenQueue.empty() && tokenQueue.front().getOp() == ')') {
                throw incorrect_expression("Two brackets!");
            }
            
            operatorStack.push(token.getOp());
        }
        else if (token.getOp() == ')') {
            if (operatorStack.empty()) {
                throw incorrect_expression("Invalid brackets!");
            }

            while (operatorStack.top() != '(') {
                char operation = operatorStack.top();
                operatorStack.pop();

                if (outputStack.empty()) {
                    throw incorrect_expression("Incorrect expression!");
                }
                double right = outputStack.top();
                outputStack.pop();

                if (outputStack.empty()) {
                    throw incorrect_expression("Incorrect expression!");
                }
                double left = outputStack.top();
                outputStack.pop();

                outputStack.push(operators.get(operation).calculate(left, right));

                if (operatorStack.empty()) {
                    throw incorrect_expression("Invalid brackets!");
                }
            }

            operatorStack.pop();
        }
    }

    while (!operatorStack.empty()) {
        if (operatorStack.top() == '(') {
            throw incorrect_expression("No closing bracket!");
        }
        char operation = operatorStack.top();
        operatorStack.pop();

        if (outputStack.empty()) {
            throw incorrect_expression("Incorrect expression!");
        }
        double right = outputStack.top();
        outputStack.pop();

        if (outputStack.empty()) {
            throw incorrect_expression("Incorrect expression!");
        }
        double left = outputStack.top();
        outputStack.pop();

        outputStack.push(operators.get(operation).calculate(left, right));
    }

    if (outputStack.empty()) {
        throw incorrect_expression("Incorrect expression!");
    }
    return outputStack.top();
}

OperationSet parseOperators(std::istream& ops) {
    OperationSet operators;

    while (!ops.eof()) {
        char operationCharacter;
        char operation;
        double priority;
        char associativity;

        ops >> operationCharacter >> operation >> priority >> associativity;

        bool assoc = associativity == 'L' ? false : true;

        operators.addOperation(operation, operationCharacter, priority, assoc);
        
        ops.get();
    }

    return operators;
}

double evaluate(const char* expression, std::istream& ops) {
    if (expression == nullptr) {
        throw incorrect_expression("Expression is empty!");
    }
    OperationSet operators = parseOperators(ops);

    return calculate(expression, operators);
}

