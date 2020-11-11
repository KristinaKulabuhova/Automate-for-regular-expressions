#include <iostream>
#include <unordered_set>
#include <iterator>
#include <fstream>
#include "test.h"
#include "automate.h"
#include "automate_builder.h"

struct BeenData
{
    int x;
    int vert;
    int eps;

    BeenData() = default;
    BeenData(int x, int vert, int eps) : x(x), vert(vert), eps(eps) {}

};

bool operator==(const BeenData& first, const BeenData &second)
{
    return first.x == second.x && first.vert == second.vert && first.eps == second.eps;
}

class Hash
{
public:
    size_t operator()(const BeenData &value) const
    {
        return value.x + value.vert + value.eps;
    }
};

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


void allGood(Automate automate, char symbol, int k, int vert, int eps, int max_eps, int finish_vertex, std::unordered_set<int> &good_vert, std::unordered_set<BeenData, Hash> &been)
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
        been.insert(BeenData(k, vert, eps));
        //return;
    } 
    else 
    {
        auto it = automate.vertices.end();
        std::advance(it, static_cast<size_t>(vert));

        for (auto map_it = (*it).begin(); map_it != (*it).end(); ++map_it)
        {
            if (map_it->first == 'e' && eps + 1 < max_eps)
            {
                for (auto &el : map_it->second)
                {
                    if (!been.contains(BeenData(k, el, eps + 1)))
                    {
                        been.insert({k, el, eps + 1});
                        allGood(automate, symbol, k, el, eps + 1, max_eps, finish_vertex, good_vert, been);
                    }
                }
            }

            if (map_it->first == symbol)
            {
                for (auto &el : map_it->second)
                {
                    if (!been.contains(BeenData(k - 1, el, eps)))
                    {
                        been.insert({k, el, eps + 1});
                        allGood(automate, symbol, k - 1, el, 0, max_eps, finish_vertex, good_vert, been);
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
    allGood(automate, symbol, n, cur, 0, max_eps, finish_vertex, good_vert, been);

    return good_vert.size() != 0;
}

void Test() 
{
    
    if (Test_concatenation())
    {
        std::cout << "Test №1 passed\n";
    }
    else {
        std::cout << "Test №1 failed\n";
    }

    if (Tets_operator_plus())
    {
        std::cout << "Test №2 passed\n";
    }
    else {
        std::cout << "Test №2 failed\n";
    }

    if (Test_closure())
    {
        std::cout << "Test №3 passed\n";
    }
    else {
        std::cout << "Test №3 failed\n";
    }

    if (Test_get_number_of_vertices())
    {
        std::cout << "Test №4 passed\n";
    }
    else {
        std::cout << "Test №4 failed\n";
    }

    if (Test_operator_plus())
    {
        std::cout << "Test №5 passed\n";
    }
    else {
        std::cout << "Test №5 failed\n";
    }

    if (Test_IsCorrectRegularExpression())
    {
        std::cout << "Test №6 passed\n";
    }
    else {
        std::cout << "Test №6 failed\n";
    }

    if (Test_mergeVertices())
    {
        std::cout << "Test №7 passed\n";
    }
    else {
        std::cout << "Test №7 failed\n";
    }
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

int main()
{
    int k = 0;
    char x = 0;
    std::string expression;
    std::cin >> expression;
    if (!IsCorrectRegularExpression(expression))
    {
        throw IncorrectRegularExpression();
    }
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

    auto automate = builder.top();
    int finish_vertex = automate.get_terminal();
    Automate invert_automate = automate.invert();
    dump(invert_automate);
    std::cin >> x >> k;
    std::vector<size_t> answer_vertices;
    for (int i = 0; i < finish_vertex + 1; ++i)
    {
        if (canGetPrefix(k, i, automate, finish_vertex + 1, x, finish_vertex))
        {
            answer_vertices.push_back(i);
        }
    }

    if (answer_vertices.size() == 0)
    {
        std::cout << "INF" << "\n";
    }
    else
    {
        std::vector<std::vector<int>> dijkstra_automate = prepare_for_dijkstra(automate);
        std::vector<int> distance = dijkstra(finish_vertex + 1, 0, dijkstra_automate);
        std::vector<int> all_distance;
        all_distance.reserve(answer_vertices.size());
        for (int j = 0; j < answer_vertices.size(); ++j)
        {
            std::vector<int> dist = dijkstra(finish_vertex + 1, j, dijkstra_automate);
            all_distance[j] = dist[finish_vertex];
        }
        size_t min = 10000;
        for (int l = 0; l < answer_vertices.size(); ++l)
        {
            if (all_distance[l] + k < min)
            {
                min = all_distance[l] + k;
            }
        }
        std::cout << min << "\n";
    }
    Test();
}
