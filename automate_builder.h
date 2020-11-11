#ifndef AUTOMATE_BUILDER_H
#define AUTOMATE_BUILDER_H

#include<stack>
#include "automate.h"

class AutomateBuilder
{
private:
    std::stack<Automate> automates;

public:
    void operator+=(char symbol);

    void concatenate();
    void unificate();
    void closure();

    Automate top();

    friend bool Test_operator_plus(size_t n);
};

#endif