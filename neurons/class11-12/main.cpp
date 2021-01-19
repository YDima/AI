#include <vector>
#include <memory>
#include <iostream>
#include <functional>

using namespace std;

class neuron {
public:
    string name;
    vector<neuron> input;
    vector<double> w;
    function<double(double)> activation;
    
    neuron (string n) {
        name = n;
        activation = [](double v) {return (v > 0) ? 1.0 : 0.0;};
    }

    double value() {
        return activation(sum());
    }

    double sum() {
        if (input.size() != w.size())
            throw "Inputs and weights are different";
        double sum = 0;
        for (int x = 0; x < input.size(); x++) {
            neuron n = input.at(x);
            sum += n.value() * w[x];
        }
        return sum;
    }

    void print() {
        cout<<"Neuron "<<name<<": "<<endl;
        cout<<" Sum: "<<sum()<<endl;
        cout<<" Activation: "<<value()<<endl;
        for (int x = 0; x < w.size(); x++) {
            cout<<" w: "<<w.at(x)<<endl;
        }
        cout<< " inputs: " << endl;
        for (int x = 0; x < input.size(); x++) {
            neuron n = input.at(x);
            cout<<"  "<<n.name<<endl;
        }
        cout<<endl;
    }

};

int main() {

    neuron input1 = neuron("input1");
    input1.activation = [](double v){return 1;};

    neuron input2 = neuron("input2");
    input2.activation = [](double v){return 0;};

    neuron n1 = neuron("n1");
    n1.input = {input1};
    n1.w = {1};
    n1.print();
    
    neuron n2 = neuron("n2");
    n2.input = {input2};
    n2.w = {1};
    n2.print();

    neuron n3 = neuron("n3");
    n3.input = {n1, n2};
    n3.w = {1, 1};
    n3.print();

    cout<<"Network: "<< n3.value() <<endl;

    // n3.input[0].input[0].activation = [](double v){return 0;};
    // cout<<"Network: "<< n3.value() <<endl;
}
