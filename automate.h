#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <list>
#include <unordered_map>
#include <stack>

using Verts = std::list<std::unordered_map<char, std::list<size_t>>>;

class IncorrectRegularExpression
{
public:
    const char *what() const noexcept;
};

class Automate
{
private:
    size_t n_vertices;
    size_t term_idx;

public:
    Verts vertices;

    Automate(const char symbol);

    Automate(size_t n, size_t idx, Verts other)
    {
        auto other_it = other.begin();
        for (; other_it != other.end(); ++other_it)
        {
            this->vertices.push_back(*other_it);
        }
        this->n_vertices = n;
        this->term_idx = idx;
    }
    ~Automate();

    
    Automate &operator+=(Automate &other);

    void concatenation(Automate &other);
    void closure();

    
    Automate &operator+=(size_t n);

    void mergeVertices(Verts &first, Verts &second);
    size_t get_number_of_vertices() const;
    size_t get_terminal();

    friend bool Test_concatenation();
    friend bool equal_automate(Automate first, Automate second);
    friend bool Test_closure();
    friend bool Tets_operator_plus();
    friend bool Test_get_number_of_vertices();
    friend bool Test_operator_plus(size_t n);
    friend bool Test_mergeVertices(Verts &first, Verts &second);
};

std::vector<int> dijkstra(int n_verts, int start, std::vector<std::vector<int>> matrix);
std::vector<std::vector<int>> prepare_for_dijkstra(const Automate &automate);
bool IsCorrectRegularExpression(std::string regular);

class AutomateBuilder
{
private:
    std::stack<Automate> automates;

public:
    void operator+=(char symbol)
    {
        automates.emplace(symbol);
    }
    
    void concatenate()
    {
        auto first = automates.top();
        automates.pop();
        auto second = automates.top();
        automates.pop();
        first.concatenation(second);

        automates.push(first);
    }

    void unificate()
    {
        auto first = automates.top();
        automates.pop();
        auto second = automates.top();
        automates.pop();
        first += second;

        automates.push(first);
    }

    void closure()
    {
        auto automate = automates.top();
        automates.pop();
        automate.closure();

        automates.push(automate);
    }

    Automate top()
    {
        return automates.top();
    }
};



#endif