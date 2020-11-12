#ifndef SOLVE_H
#define SOLVE_H

#include <iostream>
#include <unordered_set>
#include <iterator>
#include <fstream>
#include "automate.h"
#include "automate_builder.h"

struct edge{
    size_t begin;
    size_t end;
    size_t cost;
};

struct BeenData
{
    int x;
    long unsigned int vert;

    BeenData() = default;
    BeenData(int x, long unsigned vert) : x(x), vert(vert) {}

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

void allGood(Automate automate, char symbol, int count, int vert, std::unordered_set<int> &good_vert, std::unordered_set<BeenData, Hash> &been);
Automate buildAutomate(std::string expression);
bool canGetPrefix(int n, int cur, Automate automate, char symbol);
std::vector<size_t> findGoodVertices(Automate automate, int count, char symbol); 
bool can_get_terminate_eps(Automate automate, int vert, std::unordered_set<int> &been_to_eps);
std::vector<size_t> findMinDist(int start, Automate automate, std::vector<size_t> answer_vertices);
size_t Answer(std::string expression, char symbol, int count);
void dump(Automate automate);

bool can_get_terminate_eps(Automate automate, int vert, std::unordered_set<int> &been_to_eps) 
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
                    if (el == automate.get_terminal()) 
                        return true;
                    
                    

                    if (!been_to_eps.contains(el))
                    {
                        been_to_eps.insert(el);
                        can_get = can_get_terminate_eps(automate, el, been_to_eps);
                    }
                }
                break;
            }
        }
    return can_get;
}

size_t Answer(std::string expression, char symbol, int count)
{
    size_t answer = 0;
    Automate automate = buildAutomate(expression);
    int finish_vertex = automate.get_terminal();

    dump(automate);

    std::vector<size_t> answer_vertices = findGoodVertices(automate, count, symbol);

    int min = INT32_MAX;
    if (answer_vertices.size() == 0)
    {
        return 0;
    }
    else 
    {
        std::vector<size_t> dist = findMinDist( 0, automate, answer_vertices);
        for (int i = 0; i < answer_vertices.size(); ++i)
        {
            if (dist[answer_vertices[i]] < min)
            {
                min = dist[answer_vertices[i]];
            }
        }
    }

    return min + count;
}

Automate buildAutomate(std::string expression)
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

void allGood(Automate automate, char symbol, int count, int vert, std::unordered_set<int> &good_vert, std::unordered_set<BeenData, Hash> &been)
{
    if (count == 0)
    {
        if (vert == automate.get_terminal())
        {
            been.insert(BeenData(count, vert));
            good_vert.insert(vert);
        } 
        else 
        {
            std::unordered_set<int> been_to_eps;
            if (can_get_terminate_eps(automate, vert, been_to_eps))
            {
                good_vert.insert(vert);
            }
        }
        been.insert(BeenData(count, vert));
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
                    if (!been.contains(BeenData(count, el)))
                    {
                        been.insert({count, el});
                        allGood(automate, symbol, count, el, good_vert, been);
                    }
                }
            }

            if (map_it->first == symbol)
            {
                for (auto &el : map_it->second)
                {
                    if (!been.contains(BeenData(count - 1, el)))
                    {
                        been.insert({count, el});
                        allGood(automate, symbol, count - 1, el, good_vert, been);
                    }
                }
            }
        }
    }

}

bool canGetPrefix(int count, int cur, Automate automate, char symbol)
{
    std::unordered_set<int> good_vert;
    std::unordered_set<BeenData, Hash> been;
    allGood(automate, symbol, count, cur, good_vert, been);

    return good_vert.size() != 0;
}

std::vector<size_t> findGoodVertices(Automate automate, int count, char symbol)
{
    std::vector<size_t> answer_vertices;
    for (int i = 0; i < automate.get_number_of_vertices(); ++i)
    {
        if (canGetPrefix(count, i, automate, symbol))
        {
            answer_vertices.push_back(i);
        }
    }
    return answer_vertices;
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

std::vector<size_t> findMinDist(int start, Automate automate, std::vector<size_t> answer_vertices)
{
    std::vector<edge> edges;
    size_t i = 0;
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

    const int INF = INT32_MAX;
    std::vector<size_t> dist(automate.get_number_of_vertices(), INF);
    dist[start] = 0;
    for(int i = 0; i < automate.get_number_of_vertices() - 1; ++i)
    {
        for (int j = 0; j < edges.size(); ++j)
        {
            dist[edges[j].end] = std::min(dist[edges[j].end], dist[edges[j].begin] + edges[j].cost);
        }
    }
    return dist;
}

#endif