#include <iostream> // I/O Library
#include <vector>   // Vectors Structure
#include <stdlib.h> // Randomizer
#include <time.h>   // Time
#include <assert.h> // Assertion Library
#include <math.h>

#define population_length 5
#define mutation_rate 0.01
#define generations_limit 300
#define generations_stagnant 20
#define time_limit 1000

using namespace std;

#define inf 2147483647

struct vertex
{
    int name;
    float x;
    float y;
};

struct solution
{
    vector<vertex> vertexs;
    float solution_cost;
};

struct genetic_structure
{
    vector<vertex> vertexs;
    vector<solution> population;
};

vertex split(string entry, char separator)
{
    size_t end = 0;
    vertex result;
    vector<float> aux;

    while (entry.at(0) == ' ')
        entry.erase(0, 1);

    end = entry.find(separator, 0);
    result.name = stoi(entry.substr(0, end));
    entry.erase(0, end);

    while (entry.at(0) == ' ')
        entry.erase(0, 1);

    while ((end = entry.find(separator, 0)) != string::npos)
    {
        aux.push_back((stof(entry.substr(0, end), nullptr)));
        entry.erase(0, end);

        while (entry.at(0) == ' ')
            entry.erase(0, 1);
    }

    aux.push_back((stof(entry.substr(0), nullptr)));

    result.x = aux[0];
    result.y = aux[1];
    return result;
}

vector<vertex> read_entry()
{
    vector<vertex> answer;
    string in;

    for (int i = 0; i < 7; i++)
        getline(cin, in);

    while (in != "EOF")
    {
        answer.push_back(split(in, ' '));
        getline(cin, in);
    }
    setbuf(stdin, NULL);

    return answer;
}

float calculate_cost(vertex u, vertex v)
{
    return sqrt(pow(v.x - u.x, 2) + pow(v.y - u.y, 2));
}

float calculate_total_cost(vector<vertex> v)
{
    float total = 0;

    for (int i = 0; i < v.size()-1; i++)
        total += calculate_cost(v.at(i), v.at(i + 1));

    return total;
}

void create(genetic_structure *g)
{
    int random;
    for (int i = 0; i < population_length; i++)
    {
        solution x;
        vector<vertex> aux = g->vertexs;

        while (!aux.empty())
        {
            random = rand() % aux.size();
            x.vertexs.push_back(aux.at(random));
            aux.erase(aux.begin() + random);
        }
        x.solution_cost = calculate_total_cost(x.vertexs);
        g->population.push_back(x);
    }
}

solution genetic_algorithm(vector<vertex> entry)
{
    genetic_structure g;
    g.vertexs = entry;
    create(&g);

    solution s;
    return s;
}

main()
{
    srand(time(NULL));
    vector<vertex> entry = read_entry();
    solution s = genetic_algorithm(entry);
}