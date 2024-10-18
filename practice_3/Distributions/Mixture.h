#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cstdlib>
#include "./Primary.h"

class Mixture
{
private:
    Primary first;
    Primary second;
    double p;
public:
    double get_density(double &x);
    double* get_parametrs();
    double random_value();
    void first_set_nu(double &value);
    void first_set_mu(double &value);
    void first_set_lambda(double &value);
    void second_set_nu(double &value);
    void second_set_mu(double &value);
    void second_set_lambda(double &value);
    void set_p(const double &value);
    double* get_mixture_values();
    void save(string &filename);
    void load(string &filename);
    Mixture(double nu1, double mu1, double lambda1, double nu2, double mu2, double lambda2, double propose);
};
