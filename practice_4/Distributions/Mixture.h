#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include "./Primary.h"

template <class T1, class T2>
class Mixture : public IDistribution, public IPersistent
{
private:
    T1 first;
    T2 second;
    double p;

public:
    Mixture(T1 fDist, T2 sDist, double propose) : first(fDist), second(sDist), p(propose) {}
    double *get_mixture_values()
    {
        double *result;
        result[0] = first.get_nu();
        result[1] = first.get_mu();
        result[2] = first.get_lambda();
        result[3] = second.get_nu();
        result[4] = second.get_mu();
        result[5] = second.get_lambda();
        result[6] = p;
        return result;
    }
    double get_density(double x) override
    {
        return p * first.get_density(x) + (1 - p) * second.get_density(x);
    }
    double *get_parametrs() override
    {
        double *result = new double[4];
        double *first_parametrs = new double[4];
        double *second_parametrs = new double[4];
        first_parametrs = first.get_parametrs();
        second_parametrs = second.get_parametrs();
        result[0] = (p) * (first_parametrs[0]) + (1 - p) * (second_parametrs[0]);
        result[1] = (p * (pow(first_parametrs[0], 2) + first_parametrs[1]) + ((1 - p) * (pow(second_parametrs[0], 2) + second_parametrs[1]))) - pow(result[0], 2);
        result[2] = 0.0;
        result[3] = ((p * pow(first_parametrs[0] - result[0], 4) + 6 * pow(first_parametrs[0] - result[0], 2) * result[1] + pow(first_parametrs[1], 2) * (first_parametrs[3] + 3)) + ((1 - p) * pow(second_parametrs[0] - result[0], 4) + 6 * pow(second_parametrs[0] - result[0], 2) * result[1] + pow(second_parametrs[1], 2) * (second_parametrs[3] + 3))) / pow(result[1], 2) - 3;
        return result;
    }
    double random_value() override
    {
        double r1 = first.random_number();
        if (r1 > p)
        {
            return first.random_value();
        }
        else
        {
            return second.random_value();
        }
    }
    void first_set_nu(double &value)
    {
        first.set_nu(value);
    }
    void first_set_mu(double &value)
    {
        first.set_mu(value);
    }
    void first_set_lambda(double &value)
    {
        first.set_lambda(value);
    }
    void second_set_nu(double &value)
    {
        second.set_nu(value);
    }
    void second_set_mu(double &value)
    {
        second.set_mu(value);
    }
    void second_set_lambda(double &value)
    {
        second.set_lambda(value);
    }
    void set_p(const double &value)
    {
        p = value;
    }
    void save(string &filename) override
    {
        ofstream output(filename);
        output << first.get_nu() << "\n";
        output << first.get_mu() << "\n";
        output << first.get_lambda() << "\n";
        output << second.get_nu() << "\n";
        output << second.get_mu() << "\n";
        output << second.get_lambda() << "\n";
        output << p;
        output.close();
        return;
    }
    void load(string &filename) override
    {
        ifstream input(filename);
        double tempNu1, tempMu1, tempLambda1, tempNu2, tempMu2, tempLambda2, tempP;
        input >> tempNu1;
        input >> tempMu1;
        input >> tempLambda1;
        input >> tempNu2;
        input >> tempMu2;
        input >> tempLambda2;
        input >> tempP;
        first.set_nu(tempNu1);
        first.set_mu(tempMu1);
        first.set_lambda(tempLambda1);
        second.set_nu(tempNu2);
        second.set_mu(tempMu2);
        second.set_lambda(tempLambda2);
        p = tempP;
        return;
    }
};