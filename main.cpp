#include <iostream>
#include <unordered_set>
#include <iterator>
#include <fstream>
#include "solve.h"
#include "test.h"
#include "automate.h"
#include "automate_builder.h"

#ifdef TEST_BUILD
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

    if (Test_Answer())
    {
        std::cout << "Test №8 passed\n";
    }
    else {
        std::cout << "Test №8 failed\n";
    }

    if (Test_buildAutomate)
    {
        std::cout << "Test №9 passed\n";
    }
    else {
        std::cout << "Test №9 failed\n";
    }

    if (Test_findMinDist)
    {
        std::cout << "Test №10 passed\n";
    }
    else {
        std::cout << "Test №10 failed\n";
    }

    if (Test_findGoodVertices)
    {
        std::cout << "Test №11 passed\n";
    }
    else {
        std::cout << "Test №11 failed\n";
    }

    if (Test_canGetPrefix)
    {
        std::cout << "Test №12 passed\n";
    }
    else {
        std::cout << "Test №12 failed\n";
    }
}
#endif

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
    #ifdef TEST_BUILD
    Test();
    #endif

    int k = 0;
    char x = 0;
    std::string expression;

    std::cin >> expression;
    std::cin >> x >> k;

    if (!IsCorrectRegularExpression(expression))
    {
        throw IncorrectRegularExpression();
    }

    Automate automate = buildAutomate(expression);
    size_t answer = Answer(automate, x, k);

    #ifdef DUMP
    dump(automate);
    #endif

    if (answer == 0)
    {
        std::cout << "INF\n";
    }
    else 
    {
        std::cout << answer << "\n";
    }  
}