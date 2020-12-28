#include "automate.h"

const char *IncorrectRegularExpression::what() const noexcept
{
    return "Incorrect regular expression";
}

bool IsCorrectRegularExpression(std::string regular)
{
    int counter = 0;

    for (int i = 0; i < regular.size(); ++i)
    {
        if (regular[i] == 'a' || regular[i] == 'b' || regular[i] == 'c' || regular[i] == '1')
        {
            ++counter;
        }
        else if (regular[i] == '.' || regular[i] == '+')
        {
            --counter;
        }
        if (counter < 1)
        {
            return false;
        }
    }
    return counter == 1;
}

Automate::Automate() = default;
Automate::Automate(const char symbol)
{
    vertices = {
        {{symbol, {1}}},
        {{'e', {}}}};
    n_vertices = 2;
    term_idx = 1;
}
Automate::Automate(int n, int idx, Verts other)
{
    auto other_it = other.begin();
    for (; other_it != other.end(); ++other_it)
    {
        this->vertices.push_back(*other_it);
    }
    this->n_vertices = n;
    this->term_idx = idx;
}
Automate::Automate(const Automate& other) : vertices(other.vertices), n_vertices(other.n_vertices), term_idx(other.term_idx){}

Automate::~Automate() = default;

Automate &Automate::operator+=(Automate &other)
{
    *this += 1;
    vertices.push_front({{'e', {1, n_vertices + 1}}});
    vertices.back()['e'].push_back(n_vertices + other.n_vertices + 1);

    other.vertices.back()['e'].push_back(other.n_vertices);
    other += n_vertices + 1;

    mergeVertices(vertices, other.vertices);
    vertices.push_back({{'e', {}}});
    n_vertices += other.n_vertices + 2;
    term_idx = n_vertices - 1;
    return *this;
}

void Automate::concatenation(Automate &other)
{
    other += n_vertices;
    mergeVertices(vertices, other.vertices);

    auto it = vertices.begin();
    std::advance(it, term_idx);
    (*it)['e'].push_back(n_vertices);

    n_vertices += other.n_vertices;
    term_idx = n_vertices - 1;
}

void Automate::closure()
{
    *this += 1;
    vertices.push_front({{'e', {1, n_vertices + 1}}});
    vertices.back()['e'].push_back(n_vertices + 1);
    vertices.push_back({{'e', {0}}});
    n_vertices += 2;
    term_idx = n_vertices - 1;
}

void Automate::mergeVertices(Verts &first, Verts &second)
{
    for (auto &elem : second)
    {
        first.push_back(elem);
    }
}

Automate &Automate::operator+=(int n)
{
    for (auto &el : vertices)
    {
        for (auto &letter : el)
        {
            for (auto &transition : letter.second)
            {
                transition += n;
            }
        }
    }
    term_idx += n;
    return *this;
}

int Automate::get_terminal() const
{
    return term_idx;
}

int Automate::get_number_of_vertices() const
{
    return this->get_terminal() + 1;
}