#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <functional>
#include <fstream>
#include <sstream>
#include <string_view>
#include <vector>
#include <bitset>
#include <time.h>

using namespace std;

using big = unsigned long long int;

random_device r;

default_random_engine e1(r());

std::mt19937 rand_gen(r());



auto genetic_algorithm = [](
        auto calculate_population_fitness,
        auto fitness,
        auto generate_population,
        auto generator,
        auto term_condition,
        auto selection,
        auto crossover,
        auto mutation,
        auto goal,
        double domain_max,
        auto iterator,
        auto convertGtoP,
        int elite_size = 0) {
    
    auto population = generate_population(generator);

    population = calculate_population_fitness(population, goal, domain_max, fitness);

    while (!term_condition(population, iterator)) 
    {
        auto parents = selection(population, fitness, convertGtoP, domain_max, goal);
        
        auto offspring = crossover(parents);
        offspring = mutation(offspring);

        auto new_population = calculate_population_fitness(offspring, goal, domain_max, fitness);

        sort(population.begin(), population.end(), [](auto a, auto b) { return a.fit > b.fit; });
        for (int e = 0; e < elite_size; e++)
            new_population.push_back(population.at(e));
        sort(new_population.begin(), new_population.end(), [](auto a, auto b) { return a.fit > b.fit; });
        // for (int i = 0; i < elite_size; i++)
        //     new_population.pop_back();
        population = new_population;
    }
    return population;
};

auto term_condition_iterations = [](auto population, int &iterations_max) {
    cout << "[" << iterations_max << "] ";

    double sum_fitness = accumulate(population.begin(), population.end(), 0.0, [](auto a, auto b) { return a + b.fit; });

    cout << " { " << ((double)sum_fitness / (double)population.size()) << " } ";
        //    for (auto &e : population)
        //    {
        //        cout << " " << e.fit;
        //    }
    cout << endl;
    iterations_max--;
    if (iterations_max > 0)
        return false;
    else
        return true;
};

struct phenotype {
    double x;
    double y;
    double fit;

    phenotype() {
        this-> x = 0;
        this-> y = 0;
    }

    phenotype(double x, double y) {
        this-> x = x;
        this-> y = y;
    }
};

struct genotype {
    vector<int> bin;
    double fit;

    genotype(int n = 0) {
        bin.resize(n);
        fit = -1;
    }
    void randomize() {
        uniform_int_distribution<int> dist(0, 1);
        for (auto &e : bin)
        {
            e = dist(rand_gen);
        }
        
    }
};

genotype convertPtoG(phenotype p, double domain_max) {

    genotype genotype;
    vector<int> binary(128);

    big x = ((p.x / pow(10,20)) + (0.185/2)) * (domain_max * 10.85);
    big y = ((p.y / pow(10,20)) + (0.185/2)) * (domain_max * 10.85);

    for (int i = 128 / 2; i > 0; i--)
    {
        binary.at(i-1) = x % 2;
        x /= 2;
    }

    for (int i = 128; i > 128 / 2; i--)
    {
        binary.at(i-1) = y % 2;
        y /= 2;
    }

    for (int i = 0; i < 128; i++)
    {
        genotype.bin.push_back(binary.at(i));
    }

    return genotype;
}

phenotype convertGtoP(genotype g, double domain_max) {
    phenotype p;
    unsigned int const half = g.bin.size() / 2;

    vector<int> p_x(g.bin.rbegin(), g.bin.rbegin() + half);
    vector<int> p_y(g.bin.rbegin() + half, g.bin.rend());

    for (int i = p_x.size() - 1; i >= 0; i--) {
        p.x += p_x.at(i) * pow(2, i);
    }

    for (int i = p_y.size() - 1; i >= 0; i--) {
        p.y += p_y.at(i) * pow(2, i);
    }
    
    p.x = ((p.x / pow(10,20)) - (0.185/2)) * (domain_max * 10.85);
    p.y = ((p.y / pow(10,20)) - (0.185/2)) * (domain_max * 10.85);

    return p;
}

auto random_sampling = [](auto goal, auto fitness, auto generator, double domain_max, int iterations) {
    phenotype best;
    phenotype p = convertGtoP(generator(), domain_max);

    double current_solution = goal(p.x, p.y);
    for (int i = 0; i < iterations; i++) {
        p = convertGtoP(generator(), domain_max);
        double new_solution = goal(p.x, p.y);
        if (fitness(new_solution) > fitness(current_solution)) {
            best = p;
            current_solution = new_solution;
        }
    }
    return best;
};

auto print = [](vector<int> &c) {
        for (auto e : c)
        {
            cout << (int)e;
        }
        cout << endl;
};

int main() {

    auto himmelblau = [](double x, double y) {
        return pow(x * x + y - 11, 2.0) + pow(x + y * y - 7, 2);
    };

    auto fitness = [](auto goal) {
        return 1/(1+goal);
    };

    auto generator = [](int size = 128) {
        // uniform_int_distribution<int> uniform_dist(0, 1);
        // vector<int> result;
        // for (int i = 0; i < size; i++)
        // {
        //     result.push_back(uniform_dist(e1));
        // }
        genotype bin = genotype(size);
        return bin;
        
    };

    auto selection_tournament = [](vector<genotype> population, auto fitness, auto convertGtoP, double domain_max, auto goal) {
        vector<genotype> population1;
        for (int i = 0; i < population1.size(); i++)
        {
            uniform_int_distribution<int> dist(0, population.size() - 1);
            genotype c1 = population.at(dist(rand_gen));
            genotype c2 = population.at(dist(rand_gen));
            phenotype p;
            p = convertGtoP(c1, domain_max);
            double fitness1 = fitness(goal(p.x, p.y));
            // cout << fitness1 << endl;
            p = convertGtoP(c2, domain_max);
            double fitness2 = fitness(goal(p.x, p.y));
            // cout << fitness1 << endl;

            auto c = fitness1 > fitness2 ? c1 : c2;
            population1.push_back(c);
        }
        return population1;
    };

    auto crossover = [](vector<genotype> parents) {
        vector<genotype> population;
        uniform_int_distribution<> distrib(0, 127);
        for (int i = 0; i < parents.size(); i+2)
        {
            genotype parent1 = parents.at(i); 
            genotype parent2 = parents.at(i+1);
            int point = distrib(rand_gen);

            for (int i = 0; i < point; i++)
            {
                swap(parent1.bin.at(i), parent2.bin.at(i));
            }

            vector<genotype> descendants;

            descendants.push_back(parent1);
            descendants.push_back(parent2);
            population = descendants;
        }
        return population;
        
    };

    auto mutation = [](vector<genotype> population) {
        for (int i = 0; i < population.size(); i++)
        {
            genotype chromosome = population.at(i);
            uniform_int_distribution<> distrib(0, 127);

            int gen_num = distrib(rand_gen);

            chromosome.bin.at(gen_num) = 1 - chromosome.bin.at(gen_num);

            population.push_back(chromosome);
        }
        return population;
    };

    auto calculate_population_fitness = [](vector<genotype> p, auto goal, double domain_max, auto fitness) {
        vector<genotype> population;
        phenotype p1;
        genotype bin;
        for (int i = 0; i < p.size(); i++)
        {
            bin = p.at(i);
            p1 = convertGtoP(bin, domain_max);
            bin.fit = fitness(goal(p1.x, p1.y));
            // bin = convertPtoG(p1, domain_max);
            population.push_back(bin);
        }
        return population;
    };
    
    auto population_init = [](auto generator) {
        vector<genotype> population;
        for (int i = 0; i < 64; i++)
        {
            genotype bin = generator();
            population.push_back(bin);
        }
        return population;
    };
    
    int iterations = 100000;
    int elite = 3;
    int iterator = 100;
    
    auto ret = genetic_algorithm(calculate_population_fitness,
                                fitness,
                                population_init,
                                generator,
                                term_condition_iterations,
                                selection_tournament,
                                crossover,
                                mutation,
                                himmelblau,
                                5,
                                iterator,
                                convertGtoP,
                                elite);
}
