#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <list>
#include <unordered_map>
#include <stack>

using Verts = std::list<std::unordered_map<char, std::list<int>>>;

class IncorrectRegularExpression
{
public:
    const char *what() const noexcept;
};

class Automate
{
private:
    int n_vertices;
    int term_idx;

public:
    Verts vertices;
    
    Automate();
    Automate(const char symbol);
    Automate(int n, int idx, Verts other);
    Automate(const Automate& other);

    Automate invert();

    ~Automate();
    
    Automate &operator+=(Automate &other);
    void concatenation(Automate &other);
    void closure();
    
    Automate &operator+=(int n);

    void mergeVertices(Verts &first, Verts &second);

    int get_number_of_vertices() const;
    int get_terminal() const;

    friend bool Test_concatenation();
    friend bool equal_automate(Automate first, Automate second);
    friend bool Test_closure();
    friend bool Tets_operator_plus();
    friend bool Test_get_number_of_vertices();
    friend bool Test_mergeVertices(Verts &first, Verts &second);
};

bool IsCorrectRegularExpression(std::string regular);

#endif