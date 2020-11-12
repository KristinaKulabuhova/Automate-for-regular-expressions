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
    Automate(size_t n, size_t idx, Verts other);
    Automate(const Automate& other);

    Automate invert();

    ~Automate();
    
    Automate &operator+=(Automate &other);
    void concatenation(Automate &other);
    void closure();
    
    Automate &operator+=(size_t n);

    void mergeVertices(Verts &first, Verts &second);

    size_t get_number_of_vertices() const;
    size_t get_terminal() const;

    friend bool Test_concatenation();
    friend bool equal_automate(Automate first, Automate second);
    friend bool Test_closure();
    friend bool Tets_operator_plus();
    friend bool Test_get_number_of_vertices();
    friend bool Test_mergeVertices(Verts &first, Verts &second);
};

bool IsCorrectRegularExpression(std::string regular);

#endif