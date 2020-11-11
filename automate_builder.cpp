#include "automate_builder.h"
#include "automate.h"

void AutomateBuilder::operator+=(char symbol)
{
    automates.emplace(symbol);
}

void AutomateBuilder::concatenate()
{
    auto first = automates.top();
    automates.pop();
    auto second = automates.top();
    automates.pop();
    second.concatenation(first);

    automates.push(second);
}

void AutomateBuilder::unificate()
{
    auto first = automates.top();
    automates.pop();
    auto second = automates.top();
    automates.pop();
    second += first;

    automates.push(second);
}

void AutomateBuilder::closure()
{
    auto automate = automates.top();
    automates.pop();
    automate.closure();

    automates.push(automate);
}

Automate AutomateBuilder::top()
{
    return automates.top();
}