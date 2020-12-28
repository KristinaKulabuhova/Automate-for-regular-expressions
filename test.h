#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <list>
#include <unordered_map>
#include <stack>
#include <algorithm>
#include "solve.h"

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
    int n_vertices = 4;
    int term_idx = 3;

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
    int n_vertices = 6;
    int term_idx = 5;

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
    int n_vertices = 4;
    int term_idx = 3;

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
    int n_vertices = 6;
    int term_idx = 5;

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
         {{'a', {3}}},
         {{'e', {}}}};
    int n_vertices = 2;
    int term_idx = 3;

    Automate result(n_vertices, term_idx, answer);

    return equal_automate(first, result);
}

bool Test_IsCorrectRegularExpression()
{
    struct regular
    {
        std::string expression;
        bool res;
    };

    std::vector<regular> regulars;
    regulars.push_back({"bc+c.aba.*.bac.+.+*", true});
    regulars.push_back({"acb...bab.c.*.ab.ba.+.+*a.", false});
    regulars.push_back({"acb-.bab.c.*.ab.ba.+.+*a.", false});
    regulars.push_back({"+acbg..bab.c.*.ab.ba.+.+*a.", false});
    regulars.push_back({"+acb..baba.c.*.ab.ba.+.+*a.", false});
    regulars.push_back({"ab+c.aba.*.bac.+.+*baa+.*+a*.", true});
    for (int i = 0; i < regulars.size(); ++i)
    {
        if (regulars[i].res != IsCorrectRegularExpression(regulars[i].expression))
        {
            return false;
        }
    }
    return true;
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

bool Test_Answer()
{
    struct test
    {
        std::string expression;
        char x;
        int k;
        int res;
    };
    
    std::vector<Automate> automate(6);
    std::vector<test> all_tests(6);
    all_tests[0] = {"ac.aa.+", 'a', 2, 2};
    all_tests[1] = {"acb..bab.c.*.ab.ba.+.+*a.", 'a', 2, 4};
    all_tests[2] = {"ab+", 'c', 1, 0};
    all_tests[3] = {"acb..b+*b+", 'b', 4, 4};
    all_tests[4] = {"acb.+*cb.+a.", 'a', 2, 2};
    all_tests[5] = {"ab+c.aba.*.bac.+.+*", 'b', 2, 0};
    for (int i = 0; i < all_tests.size(); ++i)
    {   automate[i] = buildAutomate(all_tests[i].expression);
        if (Answer(automate[i], {all_tests[i].x, all_tests[i].k, 0}) != all_tests[i].res)
        {
            return false;
        }
    }
    return true;
}

bool Test_buildAutomate()
{
    Verts answer = {
         {{'e', {1, 3}}},
         {{'a', {2}}},
         {{'e', {5}}},
         {{'b', {4}}},
         {{'e', {5}}},
         {{'e', {}}}};
    int n_vertices = 6;
    int term_idx = 5;

    Automate result_one(n_vertices, term_idx, answer);

    AutomateBuilder builder_one;
    builder_one += 'a';
    builder_one += 'b'; 
    builder_one.unificate();
    Automate real = builder_one.top();

    bool answer_one = equal_automate(result_one, real);
    
    answer = {
         {{'a', {1}}},
         {{'e', {2}}},
         {{'b', {3}}},
         {{'e', {}}}};
    n_vertices = 4;
    term_idx = 3;

    Automate result_two(n_vertices, term_idx, answer);

    AutomateBuilder builder_two;
    builder_two += 'a';
    builder_two += 'b'; 
    builder_two.concatenate();
    real = builder_two.top();

    bool answer_two = equal_automate(result_two, real);

    answer = {
         {{'e', {1, 3}}},
         {{'a', {2}}},
         {{'e', {3}}},
         {{'e', {0}}}};
    n_vertices = 4;
    term_idx = 3;

    Automate result_three(n_vertices, term_idx, answer);

    AutomateBuilder builder_three;
    builder_three += 'a';
    builder_three += 'b'; 
    builder_three.closure();
    real = builder_three.top();

    bool answer_three = equal_automate(result_three, real);

    if (answer_one == answer_two && answer_two == answer_three && answer_one == 1)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Test_findMinDist()
{   
    Automate new_automate = buildAutomate("ab.c+*");
    struct edge
    {
        int begin;
        int end;
        int cost;
    };
    std::vector<int> dist_vertices(10);
    std::vector<int> answer_vertices(10);
    std::vector<edge> edges;

    edges.push_back({0, 1, 0});
    dist_vertices.push_back(0);
    edges.push_back({0, 9, 0});
    dist_vertices.push_back(0);
    edges.push_back({1, 2, 0});
    dist_vertices.push_back(0);
    edges.push_back({1, 6, 0});
    dist_vertices.push_back(0);
    edges.push_back({6, 7, 1});
    dist_vertices.push_back(1);
    edges.push_back({7, 8, 0});
    dist_vertices.push_back(1);
    edges.push_back({8, 9, 0});
    dist_vertices.push_back(0);
    edges.push_back({9, 0, 0});
    dist_vertices.push_back(0);
    edges.push_back({2, 3, 1});
    dist_vertices.push_back(1);
    edges.push_back({3, 4, 0});
    dist_vertices.push_back(1);
    edges.push_back({4, 5, 1});
    dist_vertices.push_back(2);
    edges.push_back({5, 8, 0});
    dist_vertices.push_back(1);

    for(int i = 0; i < answer_vertices.size(); ++i)
    {
        if (dist_vertices[i] != findMinDist(0, new_automate, answer_vertices)[i])
        {
            return false;
        }
    }
    return true;
}

bool Test_findGoodVertices()
{
    std::vector<int> real = {0, 1, 2, 3, 4, 5, 6, 7};
    Automate automate = buildAutomate("ab+c.cb.+c.");
    int count = 2;
    char symbol = 'c'; 
    std::vector<int> answers = findGoodVertices(automate, {symbol, count, 0});
    std::sort(answers.begin(), answers.end());
    for (int i = 0; i < answers.size(); ++i)
    {
        if (answers[i] != real[i])
        {
            return false;
        }
    }
    return true;
}

bool Test_canGetPrefix()
{
    Automate automate_one = buildAutomate("ab+c.cb.+c.");
    int count_one = 2;
    char symbol_one = 'c'; 
    bool answer_one = canGetPrefix(2, 0, automate_one, 'c') == true;

    Automate automate_two = buildAutomate("ab.");
    int count_two = 1;
    char symbol_two = 'c'; 
    bool answer_two = canGetPrefix(2, 0, automate_two, 'c') == false;
    if (answer_two == answer_one && answer_two == 1)
    {
        return true;
    }
    return false;
}
#endif