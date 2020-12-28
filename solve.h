#ifndef SOLVE_H
#define SOLVE_H

#include <iostream>
#include <unordered_set>
#include <iterator>
#include <fstream>
#include "automate.h"
#include "automate_builder.h"

struct edge{
    int begin;
    int end;
    int cost;
};

struct Transition
{
    char symbol; 
    int count; 
    int vert;
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
    int operator()(const BeenData &value) const
    {
        return value.x + value.vert;
    }
};

Automate buildAutomate(std::string expression);
void allGood(Automate automate, Transition transition, std::unordered_set<int> &good_vert, std::unordered_set<BeenData, Hash> &been);
bool canGetPrefix(int n, int cur, Automate automate, char symbol);
std::vector<int> findGoodVertices(Automate automate, Transition transition); 
bool can_get_terminate_eps(Automate automate, int vert, std::unordered_set<int> &been_to_eps);
std::vector<int> findMinDist(int start, Automate automate, std::vector<int> answer_vertices);
int Answer(Automate automate, Transition transition);

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

bool can_get_terminate_eps(Automate automate, int vert, std::unordered_set<int> &been_to_eps) 
{
    auto it = automate.vertices.begin();
    std::advance(it, static_cast<int>(vert));   

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

int Answer(Automate automate, Transition transition)
{
    int answer = 0;
    int finish_vertex = automate.get_terminal();

    std::vector<int> answer_vertices = findGoodVertices(automate, {transition.symbol, transition.count, 0});

    int min = INT32_MAX;
    if (answer_vertices.size() == 0)
    {
        return 0;
    }
    else 
    {
        std::vector<int> dist = findMinDist( 0, automate, answer_vertices);
        for (int i = 0; i < answer_vertices.size(); ++i)
        {
            if (dist[answer_vertices[i]] < min)
            {
                min = dist[answer_vertices[i]];
            }
        }
    }

    return min + transition.count;
}

void allGood(Automate automate, Transition transition, std::unordered_set<int> &good_vert, std::unordered_set<BeenData, Hash> &been)
{
    if (transition.count == 0)
    {
        if (transition.vert == automate.get_terminal())
        {
            been.insert(BeenData(transition.count, transition.vert));
            good_vert.insert(transition.vert);
        } 
        else 
        {
            std::unordered_set<int> been_to_eps;
            if (can_get_terminate_eps(automate, transition.vert, been_to_eps))
            {
                good_vert.insert(transition.vert);
            }
        }
        been.insert(BeenData(transition.count, transition.vert));
    } 
    else 
    {
        auto it = automate.vertices.begin();
        std::advance(it, static_cast<int>(transition.vert));

        for (auto map_it = (*it).begin(); map_it != (*it).end(); ++map_it)
        {
            if (map_it->first == 'e')
            {
                for (auto &el : map_it->second)
                {
                    if (!been.contains(BeenData(transition.count, el)))
                    {
                        been.insert({transition.count, el});
                        allGood(automate, {transition.symbol, transition.count, el}, good_vert, been);
                    }
                }
            }

            if (map_it->first == transition.symbol)
            {
                for (auto &el : map_it->second)
                {
                    if (!been.contains(BeenData(transition.count - 1, el)))
                    {
                        been.insert({transition.count, el});
                        allGood(automate, {transition.symbol, transition.count - 1, el}, good_vert, been);
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
    allGood(automate, {symbol, count, cur}, good_vert, been);

    return good_vert.size() != 0;
}

std::vector<int> findGoodVertices(Automate automate, Transition transition)
{
    std::vector<int> answer_vertices;
    for (int i = 0; i < automate.get_number_of_vertices(); ++i)
    {
        if (canGetPrefix(transition.count, i, automate, transition.symbol))
        {
            answer_vertices.push_back(i);
        }
    }
    return answer_vertices; 
}

std::vector<int> findMinDist(int start, Automate automate, std::vector<int> answer_vertices)
{
    std::vector<edge> edges;
    int i = 0;
    int cost;
    for (auto it = automate.vertices.begin(); it != automate.vertices.end(); ++it, ++i)
    {
        for (auto &[letter, vertex] : *it)
        {
            cost = (letter == 'e') ? 0 : 1;
            for (int &idx : vertex)
            {
                edges.push_back({i, idx, cost});
            }
        }
    }

    const int INF = INT32_MAX;
    std::vector<int> dist(automate.get_number_of_vertices(), INF);
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