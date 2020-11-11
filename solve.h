#ifndef SOLVE_H
#define SOLVE_H

#include <iostream>
#include <unordered_set>
#include <iterator>
#include <fstream>
#include "automate.h"
#include "automate_builder.h"

struct solve_automate
{
    Automate automate;
    char symbol;
    size_t k;
    size_t finish_vertex;
};

struct edge{
    size_t begin;
    size_t end;
    size_t cost;
};

struct BeenData
{
    int x;
    int vert;

    BeenData() = default;
    BeenData(int x, int vert) : x(x), vert(vert) {}

};

bool operator==(const BeenData& first, const BeenData &second)
{
    return first.x == second.x && first.vert == second.vert;
}

class Hash
{
public:
    size_t operator()(const BeenData &value) const
    {
        return value.x + value.vert;
    }
};

void allGood(Automate automate, char symbol, int k, int vert, int finish_vertex, std::unordered_set<int> &good_vert, std::unordered_set<BeenData, Hash> &been);
Automate build_automate(std::string expression);
bool canGetPrefix(int n, int cur, Automate automate, char symbol, int finish_vertex);
bool can_get_terminate_eps(Automate automate, int vert, int finish_vertex, std::unordered_set<int> &been_to_eps);
std::vector<size_t> good_vertices(Automate automate, int k, char x, int finish_vertex);
std::vector<edge> create_struct_of_edges(Automate automate); 
size_t solve(int n_vertices, int n_edges, int start, std::vector<edge> edges, std::vector<size_t> answer_vertices, int k);
void dump(Automate automate);
size_t Answer(std::string expression, char x, int k)
{
    size_t answer = 0;
    Automate automate = build_automate(expression);
    int finish_vertex = automate.get_terminal();

    dump(automate);

    std::vector<size_t> answer_vertices = good_vertices(automate, k, x, finish_vertex);

    if (answer_vertices.size() == 0)
    {
        answer = 0;
    }
    else 
    {
        std::vector<edge> edges = create_struct_of_edges(automate);
        answer = solve(finish_vertex + 1, edges.size(), 0, edges, answer_vertices, k);
    }
    
    return answer;
}

Automate build_automate(std::string expression)
{
    AutomateBuilder builder;
    for (int i = 0; i < expression.size(); ++i)
    {
        switch (expression[i])
        {
        case 'a':
            builder += expression[i]; 
            break;
        case 'b':
            builder += expression[i];
            break;
        case 'c':
            builder += expression[i];
            break;
        case '1':
            builder += expression[i];
            break;
        case '.':
            builder.concatenate();
            break;
        case '+':
            builder.unificate();
            break;
        case '*':
            builder.closure();
            break;
        default:
            throw IncorrectRegularExpression();
            break;
        }
    }

    return builder.top();
}

bool can_get_terminate_eps(Automate automate, int vert, int finish_vertex, std::unordered_set<int> &been_to_eps) 
{
    auto it = automate.vertices.begin();
    std::advance(it, static_cast<size_t>(vert));   

    bool can_get = false;

    for (auto map_it = (*it).begin(); map_it != (*it).end(); ++map_it)
        {
            if (map_it->first == 'e')
            {
                for (auto &el : map_it->second)
                {
                    if (el == finish_vertex) 
                        return true;
                    
                    

                    if (!been_to_eps.contains(el))
                    {
                        been_to_eps.insert(el);
                        can_get = can_get_terminate_eps(automate, el, finish_vertex, been_to_eps);
                    }
                }
                break;
            }
        }
    return can_get;
}

void allGood(Automate automate, char symbol, int k, int vert, int finish_vertex, std::unordered_set<int> &good_vert, std::unordered_set<BeenData, Hash> &been)
{
    if (k == 0)
    {
        if (vert == finish_vertex)
        {
            good_vert.insert(vert);
        } 
        else 
        {
            std::unordered_set<int> been_to_eps;
            if (can_get_terminate_eps(automate, vert, finish_vertex, been_to_eps))
            {
                good_vert.insert(vert);
            }
        }
        been.insert(BeenData(k, vert));
    } 
    else 
    {
        auto it = automate.vertices.begin();
        std::advance(it, static_cast<size_t>(vert));

        for (auto map_it = (*it).begin(); map_it != (*it).end(); ++map_it)
        {
            if (map_it->first == 'e')
            {
                for (auto &el : map_it->second)
                {
                    if (!been.contains(BeenData(k, el)))
                    {
                        been.insert({k, el});
                        allGood(automate, symbol, k, el, finish_vertex, good_vert, been);
                    }
                }
            }

            if (map_it->first == symbol)
            {
                for (auto &el : map_it->second)
                {
                    if (!been.contains(BeenData(k - 1, el)))
                    {
                        been.insert({k, el});
                        allGood(automate, symbol, k - 1, el, finish_vertex, good_vert, been);
                    }
                }
            }
        }
    }

}

bool canGetPrefix(int n, int cur, Automate automate, int max_eps, char symbol, int finish_vertex)
{
    std::unordered_set<int> good_vert;
    std::unordered_set<BeenData, Hash> been;
    allGood(automate, symbol, n, cur, finish_vertex, good_vert, been);

    return good_vert.size() != 0;
}

std::vector<size_t> good_vertices(Automate automate, int k, char x, int finish_vertex)
{
    std::vector<size_t> answer_vertices;
    for (int i = 0; i < finish_vertex + 1; ++i)
    {
        if (canGetPrefix(k, i, automate, finish_vertex + 1, x, finish_vertex))
        {
            answer_vertices.push_back(i);
        }
    }
    return answer_vertices;
}

std::vector<edge> create_struct_of_edges(Automate automate) 
{
    std::vector<edge> edges;
    int i = 0;
    size_t cost;
    for (auto it = automate.vertices.begin(); it != automate.vertices.end(); ++it, ++i)
    {
        for (auto &[key, value] : *it)
        {
            cost = (key == 'e') ? 0 : 1;
            for (size_t &idx : value)
            {
                edges.push_back({i, idx, cost});
            }
        }
    }
    return edges;
}

void dump(Automate automate) 
{
    std::ofstream out("dump.dot");
    out << "digraph {\n";
    out << "rankdir=\"LR\";\n";

    auto it = automate.vertices.begin();
    for (int i = 0; it != automate.vertices.end(); ++it, ++i)
    {
        out << "node" << i << "[label=\"" << i << "\"];\n";
    }

    it = automate.vertices.begin();

    for (int i = 0; it != automate.vertices.end(); ++it, ++i)
    {
        for (auto &[key, value] : *it)
        {
            for (auto &idx : value)
            {
                out << "node" << i << " -> node" << idx << "[label=\"" << key << "\"];\n";
            }
        }
    }
    out << "}\n";
    out.close();
}

size_t solve(int n_vertices, int n_edges, int start, std::vector<edge> edges, std::vector<size_t> answer_vertices, int k)
{
    const int INF = INT32_MAX;
    std::vector<size_t> dist(n_vertices, INF);
    dist[start] = 0;
    for(int i = 0; i < n_vertices - 1; ++i)
    {
        for (int j = 0; j < n_edges; ++j)
        {
            dist[edges[j].end] = std::min(dist[edges[j].end], dist[edges[j].begin] + edges[j].cost);
        }
    }

    int min = INT32_MAX;
    for (int i = 0; i < answer_vertices.size(); ++i)
    {
        if (dist[answer_vertices[i]] < min)
        {
            min = dist[answer_vertices[i]];
        }
    }
    return min + k;
}

#endif