#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <list>
#include <unordered_map>
#include <stack>
#include "automate.h"

bool equal_automate(Automate first, Automate second) {
    auto ans_it = first.vertices.begin();
    auto real_it = second.vertices.begin();
    if (first.n_vertices != second.n_vertices || first.term_idx != second.term_idx)
    {
        return false;
    }
    for(; ans_it != first.vertices.end() && real_it != second.vertices.end(); ++ans_it, ++real_it) {
        auto ans_map_it = ans_it->begin();
        auto real_map_it = real_it->begin();
        for(; ans_map_it != ans_it->end() && real_map_it != real_it->end(); ++ans_map_it, ++real_map_it) {
            if(ans_map_it->first != real_map_it->first) {
                return false;
            }

            auto ans_list_it = ans_map_it->second.begin();
            auto real_list_it = real_map_it->second.begin();
            for(; ans_list_it != ans_map_it->second.end() && real_list_it != real_map_it->second.end(); ++ans_list_it, ++real_list_it) {
                if(*ans_list_it != *real_list_it) {
                    return false;
                }
            }
        }
    }
    return true;
}
    
bool Test_concatenation()
{
    Automate first('a');
    Automate second('b');

    first.concatenation(second);

    Verts answer = {
         {{'a', {1}}},
         {{'e', {2}}},
         {{'b', {3}}},
         {{'e', {}}}};
    size_t n_vertices = 4;
    size_t term_idx = 3;

    Automate result(n_vertices, term_idx, answer);

    return equal_automate(first, result);
}

bool Tets_operator_plus()
{
    Automate first('a');
    Automate second('b');

    first += second;

    Verts answer = {
         {{'e', {1, 3}}},
         {{'a', {2}}},
         {{'e', {5}}},
         {{'b', {4}}},
         {{'e', {5}}},
         {{'e', {}}}};
    size_t n_vertices = 6;
    size_t term_idx = 5;

    Automate result(n_vertices, term_idx, answer);

    return equal_automate(first, result);
}

bool Test_closure()
{
    Automate first('a');

    first.closure();

    Verts answer = {
         {{'e', {1, 3}}},
         {{'a', {2}}},
         {{'e', {3}}},
         {{'e', {0}}}};
    size_t n_vertices = 4;
    size_t term_idx = 3;

    Automate result(n_vertices, term_idx, answer);

    return equal_automate(first, result);
}

bool Test_get_number_of_vertices()
{
    Verts answer = {
         {{'e', {1, 3}}},
         {{'a', {2}}},
         {{'e', {5}}},
         {{'b', {4}}},
         {{'e', {5}}},
         {{'e', {}}}};
    size_t n_vertices = 6;
    size_t term_idx = 5;

    Automate result(n_vertices, term_idx, answer);
    if (n_vertices == answer.size())
    {
        return true;
    }
    else {
        return false;
    }
}

bool Test_operator_plus()
{
    int n = 2;
    Automate first('a');
    first += 2;

    Verts answer = {
         {{'a', {2}}},
         {{'e', {3}}}};
    size_t n_vertices = 2;
    size_t term_idx = 1 + 2;

    Automate result(n_vertices, term_idx, answer);

    return equal_automate(first, result);
}

bool Test_IsCorrectRegularExpression()
{
    std::string regular_one = "";
    std::string regular_two = "acb..bab.c.*.ab.ba.+.+*a.";
    std::string regular_three = "acb-.bab.c.*.ab.ba.+.+*a.";
    std::string regular_four = "+acbg..bab.c.*.ab.ba.+.+*a.";
    std::string regular_five = "+acb..baba.c.*.ab.ba.+.+*a.";
    std::string regular_six = "ab+c.aba.*.bac.+.+*baa+.*+a*.";
    return (IsCorrectRegularExpression(regular_one) == true &&
        IsCorrectRegularExpression(regular_two) == false &&
        IsCorrectRegularExpression(regular_three) == false &&
        IsCorrectRegularExpression(regular_four) == false &&
        IsCorrectRegularExpression(regular_five) == false &&
        IsCorrectRegularExpression(regular_six) == true);
}

bool Test_mergeVertices()
{   
    Automate one('a');
    Automate two('b');
    one.mergeVertices(one.vertices, two.vertices);

    Verts answer_list = {
         {{'a', {1}}},
         {{'e', {}}},
         {{'b', {1}}},
         {{'e', {}}}};
    Automate answer(2, 1, answer_list);
    return equal_automate(answer, one);
}