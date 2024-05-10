#pragma once

struct Operation {

private:
    char operation = '0';
    int priority;
    bool associativity;

public:
    Operation() = default;
    Operation(char operation, int priority, bool associativity) : operation(operation), priority(priority), associativity(associativity) {};
    bool operator< (const Operation& other) const;
    double calculate(double left, double right) const;

    friend class OperationSet;
};

class OperationSet {


    Operation operations[26]; 

public:

    void addOperation(char operation, char operationCharacter, int priority, bool associativity);
    bool contains(char operationCharacter) const;
    const Operation& get(char operationCharacter) const;
    

};


