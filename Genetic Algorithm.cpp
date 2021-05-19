#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>
#include <fstream>

#define population_length 20
#define mutation_rate 10
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
    string file_name;
    cin >> file_name;
    ifstream tsp(file_name);
    if(tsp.fail()){
        cout << "File not found";
        exit(1);
    }
    vector<vertex> answer;
    string in;

    getline(tsp,in);
    while(in != "NODE_COORD_SECTION")
        getline(tsp, in);
    getline(tsp,in);

    while (in != "EOF")
    {
        answer.push_back(split(in, ' '));
        getline(tsp, in);
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

    for (unsigned int i = 0; i < v.size() - 1; i++)
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
    int i = rand() % g->population.size();
    *parent1 = g->population[i];
    i = rand() % g->population.size();
    *parent2 = g->population[i];
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
        child->solution_cost = calculate_total_cost(child->vertexs);
    }
}

void update(genetic_structure *g, solution child)
{
    sort(g->population.begin(), g->population.end(), solution_compare);
    g->population.erase(g->population.begin()+population_length,g->population.end());

    if (child.solution_cost < g->population[g->population.size() - 1].solution_cost)
    {
        stagnant_counter = 0;
        g->population.pop_back();
        g->population.push_back(child);
        sort(g->population.begin(), g->population.end(), solution_compare);
    }
    stagnant_counter++;
}

void generate_neighborhood(genetic_structure *g)
{
    int i = rand() % (g->population.at(0).vertexs.size()/3);
    int j = rand() % (2*g->population.at(0).vertexs.size()/3) + (g->population.at(0).vertexs.size() / 3);
    vector<solution> neighborhood;
    solution aux;
    for (solution s : g->population)
    {
        aux.vertexs.clear();
        for (int n = 0; n < i; n++)
            aux.vertexs.push_back(s.vertexs[n]);
        for (int n = j; n >= i; n--)
            aux.vertexs.push_back(s.vertexs[n]);
        for (unsigned int n = j + 1; n < s.vertexs.size(); n++)
            aux.vertexs.push_back(s.vertexs[n]);
        aux.solution_cost = calculate_total_cost(aux.vertexs);
        neighborhood.push_back(aux);
    }
    for (solution s : neighborhood)
        g->population.push_back(s);
}

solution genetic_algorithm(vector<vertex> entry)
{
    genetic_structure g;
    solution parent1, parent2, child;
    g.vertexs = entry;

    create(&g);

    while (stagnant_counter < generations_stagnant)
    {
        select(&g, &parent1, &parent2);
        child = crossover(parent1, parent2);
        mutation(&child);
        generate_neighborhood(&g);
        update(&g, child);
    }

    return g.population.at(0);
}

int main()
{
    srand(time(NULL));
    vector<vertex> entry = read_entry();
    solution s = genetic_algorithm(entry);
    cout << s.solution_cost;
}