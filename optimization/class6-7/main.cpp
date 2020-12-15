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

using namespace std;

random_device r;
default_random_engine e1(r());

void saveData(vector<double> data) {
    ofstream file("result.txt");
}

auto randomize = [](double min, double max) {
    vector<double> x(2);
    uniform_real_distribution<double> uniform_dist(min,max);
    x[0] = uniform_dist(e1);
    x[1] = uniform_dist(e1);
    return x;
};

auto optimalize = [](auto goal, double min, double max) {
    vector<double> x(2);
    x = randomize(min, max);
    auto current_solution = x;
    
    for (int iteration = 0; iteration < 1000000; iteration++) {
        x = randomize(min, max);
        auto next_sol = x;
        if (goal(current_solution) > goal(next_sol)) {
            current_solution = next_sol;
        }
        file.open;
        file << iteration << " " << goal(current_solution) << endl;
    }
    return current_solution;
};

auto hill_climb = [](auto goal, double min, double max, auto get_random_neighbours, int max_iterations) {

    vector<double> x(2);
    x = randomize(min, max);
    
    auto current_solution = x;
    
    for (int iteration = 0; iteration < max_iterations; iteration++) {
        auto next_solutions = get_random_neighbours(current_solution);
        uniform_int_distribution<int> uniform_dist(0,next_solutions.size()-1);
        auto next_sol = next_solutions.at(uniform_dist(e1));
        if (goal(current_solution) > goal(next_sol)) {
            current_solution = next_sol;
        }
        // file << iteration << " " << goal(current_solution) << endl;
    }

    return current_solution;
};

int main(int argc, char** argv) {
    auto min = 0.0;
    auto max = 0.0;
    int f;
    int i;

    std::cout << "1. Sphere function;    "
    "2. Himmelblau function;    "
    "3. ThreeHumpCamel function." << endl;
    cin >> i;

    /// f(x)    x należy do R^n
    auto neighbours = [](vector<double> x, double dx = 0.001) {
        vector<vector<double>> ret;
        for (int i = 0; i < x.size(); i++)
        {
            auto nx = x;
            nx[i] += dx;
            ret.push_back(nx);
            nx[i] -= 2.0 * dx;
            ret.push_back(nx);
        }
        return ret;
    };

    auto sphere = [](vector<double> x) {
        double sum = 0;
        for (auto e : x)
        {
            sum += e * e;
        }
        return sum;
    };

    auto himmelblau = [](vector<double> d) {
        double x = d.at(0);
        double y = d.at(1);
        return pow(x * x + y - 11, 2.0) + pow(x + y * y - 7, 2);
    };

    auto threeHumpCamel = [](vector<double> d) {
            double x = d.at(0);
            double y = d.at(1);
            return 2*x*x - 1.05*pow(x, 4.0) + pow(x, 6.0)/6 + x*y + y*y;
        };
  
    auto goal = *sphere;

    if (i == 1) {
        min = -100;
        max = 100;
    }

    if (i == 2) {
        min = -5;
        max = 5;
        goal = himmelblau;
    }
    
    if (i == 3) {
        min = -5;
        max = 5;
        goal = threeHumpCamel;
    }

    auto solution = hill_climb(goal, min, max, neighbours, 100000);
    // auto solution = optimalize(goal, min, max);
    std::cout << "result: [ ";
    for (auto e: solution)
        std::cout << e << " ";
    std::cout << "] -> " << goal(solution) << endl;  

    vector<vector<double>> data;

    for (int i = 0; i < 20; i++) {
        data.push_back(solution);
    }

    const unsigned int result_size = data.at(0).size();
    
    vector<double> average;



    for (int i = 0; i < result_size; i++) {
        double sum;
        for (int j = 0; j < data.size(); j++) {
            sum = sum + data.at(j).at(i);
        }
        double a = sum / result_size;
        average.push_back(a);
    }
    
    saveData(average);
    system("gnuplot -p ex.gnu");

    return 0;
    
}