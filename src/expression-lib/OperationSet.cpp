#include "OperationSet.h"
#include "expression.h"


void OperationSet::addOperation(char operation, char operationCharacter, int priority, bool associativity) {

    if (operationCharacter >= 'A' && operationCharacter <= 'Z') {
        operationCharacter += ('a' - 'A');
    }

    operations[operationCharacter - 'a'] = Operation(operation, priority, associativity);
}

bool OperationSet::contains(char operationCharacter) const {
    if (operationCharacter >= 'A' && operationCharacter <= 'Z') {
        operationCharacter += ('a' - 'A');
    }

    return operations[operationCharacter - 'a'].operation != '0';
}

double Operation::calculate(double left, double right) const {
    
    switch (operation)
    {   

    case ('+'):
        return left + right;
        break;
    case ('-'):
        return left - right;
        break;
    case ('*'):
        return left * right;
        break;
    case ('/'):
        if (right == 0) {
            throw incorrect_expression("Invalid operation!");
        }
        return left / right;
        break;

    default:
        throw incorrect_expression("Invalid operation!");
        break;
    }
}

const Operation& OperationSet::get(char operationCharacter) const {
    return operations[operationCharacter - 'a'];
}


bool Operation::operator< (const Operation& other) const {
    if (priority == other.priority) {
        
        return !associativity;
    }
    return priority < other.priority;
}