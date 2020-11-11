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

Automate::Automate(size_t n, size_t idx, Verts other)
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

Automate Automate::invert() 
{
    std::vector<std::unordered_map<char, std::list<size_t>>> new_vector(this->vertices.size());
    auto new_it = new_vector.begin();
    int i = 0;
    for (auto it = this->vertices.begin(); it != this->vertices.end(); ++it, ++new_it, ++i)
    {
        for (auto &[key, value] : *it)
        {
            for (auto &idx : value)
            {
                new_vector[idx][key].push_back(i);
            }
        }
    }
    std::list<std::unordered_map<char, std::list<size_t>>> new_list;
    for (int i = 0; i < new_vector.size(); ++i)
    {
        new_list.push_back(new_vector[i]);
    }
    Automate new_automate(this->n_vertices, this->term_idx, new_list);
    return new_automate;
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
    term_idx += n;
    return *this;
}
size_t Automate::get_terminal()
{
    return term_idx;
}
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
std::vector<std::vector<int>> prepare_for_dijkstra(const Automate &automate)
{
    size_t size = automate.get_number_of_vertices();
    std::vector<std::vector<int>> adjacency_matrix(size, std::vector<int>(size, 0));

    auto it = automate.vertices.begin();

    for (int i = 0; i < size; ++i)
    {
        for (auto &[key, value] : *it)
        {
            if (key != 'e')
            {
                for (auto &idx : value)
                {
                    adjacency_matrix[i][idx] = 1;
                }
            }
        }
        ++it;
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
        int id = -1;
        int min_weight = INT32_MAX;

        for (int j = 0; j < n_verts; ++j)
        {
            if (valid[j] && weight[j] < min_weight)
            {
                min_weight = weight[j];
                id = j;
            }
        }

        for (int j = 0; j < n_verts; ++j)
        {
            if (weight[id] + matrix[id][j] < weight[j])
            {
                weight[j] = weight[id] + matrix[id][j];
            }
        }
        valid[id] = false;
    }

    return weight;
}
