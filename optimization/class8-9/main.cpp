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
#include <string_view>
#include <vector>
#include <bitset>
#include <time.h>

using namespace std;

using big = unsigned long long int;

random_device r;
default_random_engine e1(r());

struct phenotype {
    double x;
    double y;

    phenotype() {
        this-> x = 0;
        this-> y = 0;
    }

    phenotype(double x, double y) {
        this-> x = x;
        this-> y = y;
    }
};

vector<int> convertPtoG(phenotype p, double domain_max) {

    vector<int> genotype;
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
        genotype.push_back(binary.at(i));
    }

    return genotype;
}

phenotype convertGtoP(vector<int> g, double domain_max) {
    phenotype p;
    unsigned int const half = g.size() / 2;

    vector<int> p_x(g.begin(), g.begin() + half);
    vector<int> p_y(g.begin() + half, g.end());

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
        uniform_int_distribution<int> uniform_dist(0, 1);
        vector<int> result;
        for (int i = 0; i < size; i++)
        {
            result.push_back(uniform_dist(e1));
        }

        return result;
        
    };

    auto crossover = [](vector<int> parent1, vector<int> parent_chrom2) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, 127);

        int break_point = distrib(gen);

        for (int i = 0; i < break_point; i++)
        {
            swap(parent1.at(i), parent_chrom2.at(i));
        }

        vector<vector<int>> descendants;

        descendants.push_back(parent1);
        descendants.push_back(parent_chrom2);

        return descendants;
    };

    auto mutation = [](vector<int> chromosome) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, 127);

        int gen_num = distrib(gen);

        chromosome.at(gen_num) = 1 - chromosome.at(gen_num);

        return chromosome;
    };

    auto get_selection_tournament = [](auto pop, unsigned int tournament_size = 2) {
        auto selection_tournament = [tournament_size](auto population) {
            using namespace std;
            decltype(population) selected_specimens;
            uniform_int_distribution<int> dist(0, population.size() - 1);
            for (int i = 0; i < population.size(); i++)
            {
                decltype(population) tournament;
                for (int t = 0; t < tournament_size; t++)
                {
                    tournament.push_back(population.at(dist(rand_gen)));
                }
                sort(
                    tournament.begin(), tournament.end(), [](auto a, auto b) { return a.fit > b.fit; });
                selected_specimens.push_back(tournament.at(0));
            }
            return selected_specimens;
        };
        return selection_tournament;
    };

    auto population_init = [](int pop_size, int chrom_size, auto rand_gene) {
        vector<chromosome_t> population(pop_size);

        for (int i = 0; i < pop_size; i++)
        {
            population.push_back(chromosome_t(chrom_size));
            rand_gene(population[i], chrom_size);
        }
        return population;
    };

    int iterations = 100000;

    // phenotype p;
    // for (int i = 0; i < 3; i++) {
    //     p = random_sampling(himmelblau, fitness, generator, 5, iterations);
    //     cout << "Himmelblau: " << p.x << ", " << p.y << " -> " << himmelblau(p.x, p.y) << endl;
    // }

    cout << "Mutation" << endl;
    vector<int> bin = generator();
    cout << endl;
    print(bin);
    cout << endl;
    bin = mutation(bin);
    print(bin);
    cout << endl;

    cout << "Crossover" << endl;
    vector<int> bin1 = generator();
    vector<int> bin2 = generator();
    print(bin1);
    vector<vector<int>> descendants(2);

    descendants = crossover(bin1, bin2);
    cout << "parents:" << endl;
    print(bin1);
    print(bin2);
    cout << "descendants:" << endl;
    print(descendants.at(0));
    print(descendants.at(1));
    
}

// srand(time(0));

//     vector<char> chromosome;
//     vector<char> X;
//     vector<char> Y;
    
//     auto genotype =[](vector<char>& chromosome, int size) {
//         int g;
//         for (int i = 0; i < size; i++) {
//                 g = rand() % 2;
//                 chromosome.push_back(g);
//         }
//     };
    
//     auto slice = [](vector<char>& array, int S, int E) { 
//         auto start = array.begin() + S; 
//         auto end = array.begin() + E + 1; 
    
//         vector<char> sliced_vector(E - S + 1); 
    
//         copy(start, end, sliced_vector.begin()); 
    
//         return sliced_vector; 
//     };
   
//     auto print = [](vector<char> &c) {
//         for (auto e : c)
//         {
//             cout << (int)e;
//         }
//         cout<<endl;
//     };
    
//      auto bin_to_double = [](vector<char> arr, int size = 64){
//         reverse(arr.begin(), arr.end());
//         double sum = 0;
//         for (int i = -20; i < size - 20; i++) {
//                 sum += arr[i]*pow(2,i);
//         }
//         return sum;
        
//     };

//     auto double_to_bin = [](double number){
//         double fractpart, intpart;
//         fractpart = modf (number , &intpart);
//         printf ("%f = %f + %f \n", number, intpart, fractpart);
//     };
    
//     auto decode = [](vector<char>& chromosome, auto slice, auto bin_to_double){
//         vector<double> arr_xy(2);
//         vector<char> X = slice(chromosome,0,63);
//         vector<char> Y = slice(chromosome,64,127);
        
//         arr_xy[0] = bin_to_double(X);  //x
//         arr_xy[1] = bin_to_double(Y);   //y  
        
//         return arr_xy;
//     };

//     int size = 128;
   
//     genotype(chromosome, size);
//     X = slice(chromosome, 0, 63);
//     Y = slice(chromosome, 64, 127);
    
//     print(chromosome);
//     print(X);
//     print(Y);
    
//     cout <<""<< endl;
    
//     vector<double> xy(2);
//     xy = decode(chromosome,slice,bin_to_double);
//     double x = xy[0];
//     double y = xy[1];