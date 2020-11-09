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
    first.concatenation(second);

    automates.push(first);
}

void AutomateBuilder::unificate()
{
    auto first = automates.top();
    automates.pop();
    auto second = automates.top();
    automates.pop();
    first += second;

    automates.push(first);
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