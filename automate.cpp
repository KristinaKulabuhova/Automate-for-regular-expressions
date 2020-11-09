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

Automate::Automate(const char symbol)
{
    vertices = {
        {{symbol, {1}}},
        {{'e', {}}}};
    n_vertices = 2;
    term_idx = 1;
}

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

size_t Automate::get_number_of_vertices() const
{
    return vertices.size();
}

Automate &Automate::operator+=(size_t n)
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

    return *this;
}
size_t Automate::get_terminal()
{
    return term_idx;
}

std::vector<std::vector<int>> prepare_for_dijkstra(const Automate &automate)
{
    size_t size = automate.get_number_of_vertices();
    std::vector<std::vector<int>> adjacency_matrix(size);

    auto it = automate.vertices.begin();

    for (int i = 0; i < size; ++i)
    {
        for (auto &[key, value] : *it)
        {
            if (key == 'e')
            {
                for (auto &idx : value)
                {
                    adjacency_matrix[i][idx] = 0;
                }
            }
            else
            {
                for (auto &idx : value)
                {
                    adjacency_matrix[i][idx] = 1;
                }
            }
        }
    }
    return adjacency_matrix;
}

std::vector<int> dijkstra(int n_verts, int start, std::vector<std::vector<int>> matrix)
{
    std::vector<bool> valid(n_verts, true);
    std::vector<int> weight(n_verts, INT32_MAX - 1);

    weight[start] = 0;

    for (int i = 0; i < n_verts; ++i)
    {
        int min_weight = INT32_MAX;
        int id = -1;

        for (int j = 0; j < n_verts; ++j)
        {
            if (valid[j] && weight[i] < min_weight)
            {
                min_weight = weight[i];
                id = i;
            }
        }

        for (int j = 0; j < n_verts; ++j)
        {
            if (weight[id] + matrix[id][i] < weight[i])
            {
                weight[i] = weight[id] + matrix[id][i];
            }
        }
        valid[id] = false;
    }

    return weight;
}
