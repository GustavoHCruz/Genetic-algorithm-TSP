#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <algorithm>

#define population_length 10
#define mutation_rate 10
#define generations_limit 1000000
int stagnant_counter = 0;
#define generations_stagnant 1000000

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

    for (int i = 0; i < v.size() - 1; i++)
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

bool solution_compare(solution a, solution b)
{
    return a.solution_cost < b.solution_cost;
}

// Ranking Selection
void select(genetic_structure *g, solution *parent1, solution *parent2)
{
    vector<solution> aux;
    for (int i = 0; i < population_length; i++)
    {
        aux.push_back(g->population[i]);
    }

    sort(aux.begin(), aux.end(), solution_compare);

    *parent1 = aux[0];
    *parent2 = aux[1];
}

solution crossover(solution parent1, solution parent2)
{
    solution child;
    vector<vertex> aux;
    int cut = parent1.vertexs.size() / 2;

    for (int i = 0; i < cut; i++)
    {
        aux.push_back(parent1.vertexs.at(i));
    }

    vertex v;
    int i = 0;
    bool exist;
    while (aux.size() < parent1.vertexs.size())
    {
        exist = false;
        v = parent2.vertexs[i++];
        for (vertex x : aux)
        {
            if (v.name == x.name)
                exist = true;
        }
        if (!exist)
            aux.push_back(v);
    }
    child.vertexs = aux;
    child.solution_cost = calculate_total_cost(aux);

    return child;
}

void mutation(solution *child)
{
    int aux = rand() % 10000 + 1;
    if (aux <= mutation_rate * 100)
    {
        int a = rand() % child->vertexs.size();
        int b = rand() % child->vertexs.size();
        vertex temp = child->vertexs.at(a);
        child->vertexs.at(a) = child->vertexs.at(b);
        child->vertexs.at(b) = temp;
    }
}

void update(genetic_structure *g, solution child)
{
    sort(g->population.begin(), g->population.end(), solution_compare);

    if (child.solution_cost < g->population[g->population.size() - 1].solution_cost)
    {
        stagnant_counter = 0;
        g->population.pop_back();
        g->population.push_back(child);
    }
    stagnant_counter++;
}

solution genetic_algorithm(vector<vertex> entry)
{
    genetic_structure g;
    solution parent1, parent2, child;
    g.vertexs = entry;

    create(&g);

    //for (int generations = 0; generations < generations_limit; generations++)
    while(stagnant_counter < generations_stagnant)
    {
        select(&g, &parent1, &parent2);
        child = crossover(parent1, parent2);
        mutation(&child);
        update(&g, child);
    }

    return g.population.at(0);
}

main()
{
    srand(time(NULL));
    vector<vertex> entry = read_entry();
    solution s = genetic_algorithm(entry);
    cout << s.solution_cost;

    //for(vertex v : s.vertexs)
    //{
    //    cout << "Name:" << v.name;
    //    cout << " --- [" << v.x << "," << v.y << "]" << endl; 
    //}
}